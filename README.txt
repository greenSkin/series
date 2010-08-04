// $Id: README.txt,v 1.6 2010/07/27 15:20:50 greenskin Exp $

Drupal series.module README.txt
===============================================================================

This module allows you to put nodes into series using the Drupal taxonomy.
Nodes in a series can be sorted either by editing the series directly or via
the series tab visible when editing a node within a series. The module provides
two default views which aid in displaying series (series_list & series_nodes).
The paths 'series' and 'series/%' point respectively to the default displays of
series_list and series_nodes views, however additional page displays can be
created if so desired.

A block is provided that becomes visible when viewing within a series. The
block lists the nodes in the series in order and provides links to each.

This module is currently under development and should not be used in a
production site.

Installation
------------------------------------------------------------------------------

 Required:
  - Copy series.module to modules/

  - Enable the module as usual from Drupal's admin pages.

  - Assign at least one content type to the Series vocabulary.

Credits / Contact
------------------------------------------------------------------------------

The author of this module is greenSkin.
