<?php

/**
 * @file
 * Audit your database for inline PHP in entity fields.
 */

/**
 * Implements hook_drush_command().
 */
function architecture_drush_command() {
  $items = array();
  $items['phpaudit'] = array(
    'description' => "Audit the database for PHP code.",
    'drupal dependencies' => array('architecture'),
    'aliases' => array('apa'),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function architecture_drush_help($section) {
  switch ($section) {
    case 'drush:phpaudit':
      return dt("Audits your entity fields for hidden PHP in the database.");
  }
}

/**
 *
 */
function drush_architecture_phpaudit() {
  $field_info = _architecture_list_fields();
  drush_print_table($field_info['rows'], TRUE);
  drush_print_pipe($field_info['pipe']);
}

function _architecture_list_fields() {
  $rows[] = array(
    dt('Field name'),
    dt('Field type'),
    dt('Bundles'),
    dt('Occurrences of PHP in field'),
  );
  $info = field_info_fields();
  foreach ($info as $field_name => $field) {
    $bundle_strs = array();
    $php_audit_strs = array();
    foreach ($field['bundles'] as $entity_type => $bundles) {
      $bundle_strs[] = implode(',', $bundles);
      $php_audit_strs[] = _architecture_scour_fields($field_name . '_value', 'field_data_' . $field_name, '<?php');
    }
    $row = array(
      $field_name,
      $field['type'],
      implode(' ', $bundle_strs),
      implode(' ', $php_audit_strs),
    );
    $rows[] = $row;
    $pipe[] = implode(',', $row);
  }
  return array('rows' => $rows, 'pipe' => $pipe);
}

function _architecture_scour_fields($column, $table, $needle) {
  // @todo: add some kind of sql injection protection here.
  // I'm not super worried because a user would have to somehow sneak their
  // malicious code into a field's machine name.
  $column = check_plain($column);
  $table = check_plain($table);
  $needle = check_plain($needle);

  $query = 'SELECT `entity_id`, `entity_type` FROM `' . $table . '`  WHERE `' . $column . '` LIKE \'%' . $needle . '%\'';
  try {
    $result = db_query($query)->fetchAllAssoc('entity_id');
    $result_summary = '';
    foreach ($result as $obj) {
      $id = $obj->entity_id;
      $type = $obj->entity_type;
      // If there are results, give IDs.
      $result_summary .= "$type/$id, ";
    }
    $num = count($result);
    return $num? "$num found: " . trim($result_summary, ', ') . "." : "$num found.";
  }
  catch (Exception $e) {
    // If the field is a weird type, the query'll fail. No big.
    return 'Could not evaluate field.';
  }
}
