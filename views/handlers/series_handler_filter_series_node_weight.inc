<?php
// $Id$
/**
 * Filter by whether a node weight is set.
 */
class series_handler_filter_series_node_weight extends views_handler_filter {
  function admin_summary() { }
  function operator_form() { }

  function query() {
    $table = $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$table.weight IS NOT NULL");
  }
}
