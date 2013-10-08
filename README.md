Architecture v2.x
=================

This version of the Architecture module defines Views, using data exposed via
the Views API, that give any user with the "Use Architecture" permission the
ability to survey the structure of the site.

Security
========
Many of these queries bypass access restrictions, so assign the "Use
Architecture" permission with caution!

Data export
===========
Since it is built on Views, this module supports exporting the Views data via
the Views Data Export module, and can be used with all drush commands that come
with Views and any Views-extending modules that may be enabled.

Finding PHP in fields
=====================
There is also support for searching for PHP opening tags that may be saved in
fields. This is a common need but it also requires running a great number of
queries because each field's data is a separate table in Drupal. Therefore, no
page or block display is available for this view. Instead, it is encouraged to
use the Views Data Export drush command to get this information, as attempts to
access it via the UI may fall over, particularly on sites with a great number of
fields.
