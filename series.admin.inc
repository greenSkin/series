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
  db_query("UPDATE {vocabulary} SET multiple = 0 WHERE vid = %d", $form_state['values']['series_vid']);
  drupal_set_message(t('The series vocabulary settings have been set.'));
}

function series_node_edit_form($form_state, $node) {
  $series = series_load($node);
  return series_edit_form($form_state, $series);
}

function series_edit_form($form_state, $series) {
  $form = array(
    '#theme' => 'series_edit_form',
    '#tree' => TRUE,
    '#series' => $series,
    'nodes' => array()
  );

  $form['vid'] = array(
    '#type' => 'value',
    '#value' => $series->vid
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

  $form += series_create_form($form_state, $series);

  $form['information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Information'),
    '#description' => l(t('Edit vocabulary'), 'admin/content/taxonomy/edit/vocabulary/' . $series->vid),
    '#collapsible' => TRUE,
    '#collapsed' => ($series->description) ? TRUE : FALSE
  );
  $title_value = l($series->name, 'series/'. $series->tid);
  if (user_access('administer taxonomy')) $title_value .= ' ('. l('edit', 'admin/content/taxonomy/edit/term/'. $series->tid, array('query' => drupal_get_destination())) .')';
  $form['information']['title'] = array(
    '#type' => 'item',
    '#title' => 'Series Title',
    '#value' => $title_value
  );
  $form['information']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $series->description,
    '#description' => t('A description of the series. This will appear on the series overview page.')
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('series_edit_form_submit')
  );

  return $form;
}

function series_edit_form_submit($form, &$form_state) {
  db_query("DELETE FROM {series_weight} WHERE tid = %d", $form_state['values']['tid']);
  foreach ($form_state['values']['nodes'] as $node) {
    db_query("INSERT INTO {series_weight} (tid, nid, weight) VALUES (%d, %d, %d)", $form_state['values']['tid'], $node['nid'], $node['weight']);
  }

  db_query("UPDATE {term_data} SET description = '%s' WHERE tid = %d AND vid = %d", $form_state['values']['information']['description'], $form_state['values']['tid'], $form_state['values']['vid']);

  // Clear the cache for this particular series.
  cache_clear_all('series:'. $form_state['values']['tid'], 'cache_series');
  cache_clear_all();
}

function series_create_form($form_state, $series) {
  $vid = variable_get('series_vid', 0);
  $vocabulary = taxonomy_vocabulary_load($vid);
  $types = array();
  foreach ($vocabulary->nodes as $type) {
    if (user_access('create '. $type .' content')) {
      $types[$type] = $type;
    }
  }

  $form = array();
  if (!empty($types)) {
    $form['add_field']['#tree'] = TRUE;
    $form['add_field']['#theme'] = 'series_create_form';
    if (count($types) == 1) {
      $form['add_field']['select'] = array(
        '#type' => 'hidden',
        '#value' => key($types)
      );
      $form['add_field']['add'] = array(
        '#type' => 'button',
        '#value' => t('Add another'),
        '#validate' => array('series_edit_form_create_redirect')
      );
    }
    else {
      $options = array(t('-- Select content type --'));
      foreach (node_get_types('names') as $type => $name) {
        if (isset($types[$type])) {
          $options[$type] = $name;
        }
      }
      $form['add_field']['select'] = array(
        '#type' => 'select',
        '#title' => t('Add another'),
        '#options' => $options
      );
      $form['add_field']['add'] = array(
        '#type' => 'button',
        '#value' => t('Add another'),
        '#validate' => array('series_edit_form_create_redirect'),
        '#attributes' => array('class' => 'js-hide')
      );
    }
  }
  return $form;
}

function series_edit_form_create_redirect($form, &$form_state) {
  if ($form_state['values']['add_field']['select']) {
    drupal_goto('node/add/'. $form_state['values']['add_field']['select'], array('series' => $form_state['values']['tid']));
  }
  else {
    form_set_error('add_field][select', t('Select a content type.'));
  }
}

function theme_series_edit_form($form) {
  drupal_add_tabledrag('series-nodes', 'order', 'sibling', 'node-weight');

  $headers = array(t('Title'), t('Status'), t('Weight'), t('Operations'));
  $rows = array();

  foreach (element_children($form['nodes']) as $nid) {
    $form['nodes'][$nid]['weight']['#attributes']['class'] = 'node-weight';
    $operations = array(
      l(t('View'), 'node/'. $nid)
    );
    if (user_access('edit any '. $form['#series']->nodes[$nid]['type'] .' content')) {
      $operations[] = l(t('Edit'), 'node/'. $nid .'/edit');
    }
    $status = db_result(db_query("SELECT status FROM {node} WHERE nid = %d", $nid));
    $row = array(
      array('data' => check_plain($form['#series']->nodes[$nid]['title'])),
      array('data' => ($status) ? t('Published') : t('Unpublished')),
      array('data' => drupal_render($form['nodes'][$nid])),
      array('data' => theme('item_list', $operations, NULL, 'ul', array('class' => 'links')))
    );
    $rows[] = array(
      'data' => $row,
      'class' => 'draggable'
    );
  }

  $output = drupal_render($form['information']);
  $output .= theme('table', $headers, $rows, array('id' => 'series-nodes'));
  $output .= drupal_render($form);
  return $output;
}

function theme_series_create_form($form) {
  drupal_add_js(drupal_get_path('module', 'series') .'/js/admin.js');

  return '<div class="action">'. drupal_render($form) .'</div>';
}
