<?php
// $Id: views_plugin_argument_validate_taxonomy_term.inc,v 1.6.2.1 2009/11/30 19:01:09 merlinofchaos Exp $
/**
 * @file
 * Contains the 'taxonomy term' argument validator plugin.
 */

/**
 * Validate whether an argument is an acceptable node.
 */
class series_plugin_argument_validate_series_id extends views_plugin_argument_validate {
  function validate_argument($argument) {
    if (!is_numeric($argument)) {
      return FALSE;
    }

    $vid = variable_get('series_vid', '');
    if ($vid > 0) {
      $result = db_fetch_object(db_query("SELECT * FROM {term_data} WHERE tid = %d AND vid = %d", $argument, $vid));
      if ($result) {
        return TRUE;
      }
    }
    return FALSE;
  }
}
