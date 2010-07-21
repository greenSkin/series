<?php
// $Id: views_handler_argument_numeric.inc,v 1.1 2008/09/03 19:21:28 merlinofchaos Exp $
/**
 * @file
 * Contains the numeric argument handler.
 */

/**
 * Basic argument handler for arguments that are numeric. Incorporates
 * break_phrase.
 *
 * @ingroup views_argument_handlers
 */
class series_handler_argument_node_series extends views_handler_argument_numeric {
  function query() {
    $this->ensure_my_table();

    if (!empty($this->options['break_phrase'])) {
      views_break_phrase($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }

    if (count($this->value) > 1) {
      $operator = empty($this->options['not']) ? 'IN' : 'NOT IN';
      $placeholders = implode(', ', array_fill(0, sizeof($this->value), '%d'));
      $this->query->add_where(0, "$this->table_alias.$this->real_field $operator ($placeholders)", $this->value);
    }
    else {
      $operator = empty($this->options['not']) ? '=' : '!=';
      $this->query->add_where(0, "$this->table_alias.$this->real_field $operator %d", $this->argument);
    }
  }
}
