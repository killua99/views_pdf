<?php

class views_pdf_handler_page_number extends views_handler_field {
  
  function init(&$view, $options) {
    parent::init($view, $options);
    
  }

  function query() {
    // Override parent::query() and don't alter query.
    $this->field_alias = 'pdf_page_number_'. $this->position;
  }
  

  function render($values) {
    
    if (isset($this->view->pdf) && is_object($this->view->pdf)) {
      return $this->view->pdf->getPage();
    }
    else {
      return '';
    }
    
  }
  
  function allow_advanced_render() {
    return FALSE;
  }
}
