// $Id$

Drupal.behaviors.seriesAdmin = function() {
  $('#edit-add-field-select').change(function() {
    if ($(this).val() != 0) {
      $('#edit-add-field-add').click();
    }
  });
}
