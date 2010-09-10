<?php
// $Id: views_handler_argument_term_node_tid.inc,v 1.1 2008/09/03 19:21:30 merlinofchaos Exp $
/**
 * Allow taxonomy term ID(s) as argument
 */
class series_handler_argument_series_term_id extends views_handler_argument_term_node_tid {
  function init(&$view, &$options) {
    parent::init($view, $options);

    $tid = variable_get('series_vid', '');
    $this->options['validate_argument_vocabulary'] = array($tid => $tid);
    $this->options['validate_argument_type'] = 'tid';
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['validate_type']['#type'] = 'hidden';
    $form['validate_type']['#value'] = 'taxonomy_term';

    unset($form['validate_argument_vocabulary']);

    if (isset($form['validate_argument_type'])) {
      $form['validate_argument_type']['#type'] = 'hidden';
      $form['validate_argument_type']['#value'] = $this->options['validate_argument_type'];
    }
  }
}
