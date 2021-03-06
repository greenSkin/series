<?php
// $Id$

/**
 * Implementation of hook_data().
 */
function series_views_data() {
  $data['series_weight']['table']['group'] = t('Series');
  $data['series_weight']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid'
  );
  $data['series_weight']['weight'] = array(
    'title' => t('Weight'),
    'help' => t('The weight of a node in a series.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE
    ),
    'filter' => array(
      'handler' => 'series_handler_filter_series_node_weight'
    ),
    'sort' => array(
      'handler' => 'views_handler_sort'
    )
  );
  $data['term_data']['series_vocabulary'] = array(
    'real field' => 'vid',
    'title' => t('Series vocabulary'),
    'help' => t('The Series vocabulary.'),
    'group' => t('Series'),
    'filter' => array(
      'handler' => 'series_handler_filter_series_vocabulary'
    )
  );
  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function series_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'series') .'/views/handlers'
    ),
    'handlers' => array(
      'series_handler_filter_series_node_weight' => array(
        'parent' => 'views_handler_filter'
      ),
      'series_handler_filter_series_vocabulary' => array(
        'parent' => 'views_handler_filter'
      )
    )
  );
}

/**
 * Implementation of hook_views_plugins
 */
function series_views_plugins() {
  return array(
    'argument validator' => array(
      'series_id' => array(
        'title' => t('Series ID'),
        'handler' => 'series_plugin_argument_validate_series_id',
        'path' => drupal_get_path('module', 'series') .'/views/plugins',
        'theme path' => drupal_get_path('module', 'series') .'/views/theme'
      ),
    )
  );
}
