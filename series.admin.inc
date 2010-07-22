<?php

/**
 * @file
 * Admin page functions.
 *
 * @author greenSkin
 */

function series_admin_form() {
  $options = array();
  $vocabularies = taxonomy_get_vocabularies();
  foreach ($vocabularies as $vid => $vocabulary) {
    $options[$vid] = $vocabulary->name;
  }

  $form['series_vid'] = array(
    '#type' => 'select',
    '#title' => t('Series vocabulary'),
    '#description' => t('The taxonomy vocabulary that will be considered the series.'),
    '#default_value' => variable_get('series_vid', 0),
    '#options' => $options,
    '#required' => TRUE
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'series_admin_form_submit';
  return $form;
}

function series_admin_form_submit($form, &$form_state) {
  // Ensure the newly set vocabulary has 'tags' and 'multiple select' disabled
  // and 'required' enabled.
  db_query("UPDATE {vocabulary} SET multiple = 0, required = 1 WHERE vid = %d", $form_state['values']['series_vid']);
  drupal_set_message(t('The series vocabulary settings have been set.'));
}

function series_edit_form($form_state, $node) {
  $series = series_load($node);
  $form = array(
    '#tree' => TRUE,
    '#series' => $series,
    'nodes' => array()
  );

  $form['tid'] = array(
    '#type' => 'value',
    '#value' => $series->tid
  );

  foreach ($series->nodes as $nid => $series_node) {
    $form['nodes'][$nid]['nid'] = array(
      '#type' => 'hidden',
      '#value' => $nid
    );
    $form['nodes'][$nid]['weight'] = array(
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => $series_node['weight']
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit')
  );
  return $form;
}

function series_edit_form_submit($form, &$form_state) {
  db_query("DELETE FROM {series_weight} WHERE tid = %d", $form_state['values']['tid']);
  foreach ($form_state['values']['nodes'] as $node) {
    db_query("INSERT INTO {series_weight} (tid, nid, weight) VALUES (%d, %d, %d)", $form_state['values']['tid'], $node['nid'], $node['weight']);
  }
  // Clear the cache for this particular series.
  cache_clear_all('series:'. $form_state['values']['tid'], 'cache_series');
}

function theme_series_edit_form($form) {
  drupal_add_tabledrag('series-nodes', 'order', 'sibling', 'node-weight');

  $headers = array(t('Title'), t('Weight'), t('Operations'));
  $rows = array();

  foreach (element_children($form['nodes']) as $nid) {
    $form['nodes'][$nid]['weight']['#attributes']['class'] = 'node-weight';
    $row = array(
      array('data' => check_plain($form['#series']->nodes[$nid]['title'])),
      array('data' => drupal_render($form['nodes'][$nid])),
      array('data' => l(t('Edit'), 'node/'. $nid .'/edit'))
    );
    $rows[] = array(
      'data' => $row,
      'class' => 'draggable'
    );
  }

  $output = theme('table', $headers, $rows, array('id' => 'series-nodes'));
  $output .= drupal_render($form);
  return $output;
}
