// $Id: admin.js,v 1.1 2010/07/27 15:20:50 greenskin Exp $

Drupal.behaviors.seriesAdmin = function() {
  $('#edit-add-field-select').change(function() {
    if ($(this).val() != 0) {
      $('#edit-add-field-add').click();
    }
  });
}
