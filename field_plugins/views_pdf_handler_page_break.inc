<?php

class views_pdf_handler_page_break extends views_handler_field {
  
  protected $countRecords = 0;
  
  function init(&$view, $options) {
    parent::init($view, $options);
    
  }

  function query() {
    // Override parent::query() and don't alter query.
    $this->field_alias = 'pdf_page_break_'. $this->position;
  }
  
  function option_definition() {
    $options = parent::option_definition();
    
    $options['last_row'] = array('default' => TRUE);
    
    return $options;
  }
  
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    
    $form['last_row'] = array(
      '#type' => 'checkbox',
      '#title' => t('Exclude from last row'),
      '#default_value' => $this->options['last_row'],
      '#description' => t('Check this box to not add new page on last row.'),
    );
  }

  function render($values) {
    if (isset($this->view->pdf) && is_object($this->view->pdf)) {
      if($this->options['last_row'] == TRUE && ($this->countRecords + 1 >= $this->view->numberOfRecords)) {
        return '';
      }
      
      $this->countRecords++;
      $this->view->pdf->addPage();
      return '';
    }
    
  }
  function allow_advanced_render() {
    return FALSE;
  }
}
