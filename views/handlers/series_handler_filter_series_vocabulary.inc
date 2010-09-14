<?php
// $Id$

/**
 * Simple filter to ensure we are working with the Series vocabulary.
 */

class series_handler_filter_series_vocabulary extends views_handler_filter {
  function admin_summary() {
    $vid = variable_get('series_vid', '');
    $vocabulary = taxonomy_vocabulary_load($vid);
    if ($vocabulary) {
      return '= '. check_plain($vocabulary->name);
    }
    return t('= undefined');
  }

  function options_form(&$form, &$form_state) {
    $form['series_vocabulary']['markup'] = array(
      '#prefix' => '<div class="description">',
      '#value' => t('The set Series vocabulary can be adjusted on the !link.', array('!link' => l(t('Series settings page'), 'admin/settings/series', array('query' => drupal_get_destination())))),
      '#suffix' => '</div>'
    );
  }

  function query() {
    $vid = variable_get('series_vid', '');
    if ($vid > 0) {
      $this->ensure_my_table();
      $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field = %d", $vid);
    }
  }
}
