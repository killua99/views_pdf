<?php

/**
 * @file
 * Contains \Drupal\views_pdf\Plugin\views\style\PageBreak.
 */

// We can't use name space in views 7.x-x.x
// namespace Drupal\views_pdf\Plugin\views\style;

/**
 * Class that holds the functionality for the page break in a PDF display.
 *
 * This plugin is used to add a page break to a PDF display.
 *
 * @ingroup views_field_handler
 */
class PageBreak extends views_handler_field {

  protected $countRecords = 0;

  /**
   * This method  is used to query data. In our case
   * we want that no data is queried.
   *
   */
  function query() {
    // Override parent::query() and don't alter query.
    $this->field_alias = 'pdf_page_break_' . $this->position;
  }

  /**
   * This method contains the defintion of the options for the page break.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['last_row']  = array('default' => FALSE);
    $options['every_nth'] = array('default' => 1);

    return $options;
  }

  /**
   * Option form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['last_row']  = array(
      '#type'          => 'checkbox',
      '#title'         => t('Exclude from last row'),
      '#default_value' => $this->options['last_row'],
      '#description'   => t('Check this box to not add new page on last row.'),
    );
    $form['every_nth'] = array(
      '#type'             => 'textfield',
      '#title'            => t('Insert break after how many rows?'),
      '#size'             => 10,
      '#default_value'    => $this->options['every_nth'],
      '#element_validate' => array('element_validate_integer_positive'),
      '#description'      => t('Enter a value greater than 1 if you want to have multiple rows on one page')
    );
  }

  /**
   * This method renders the page break. It uses the PDF class to add a page break.
   */
  function render($values) {
    if (isset($this->view->pdf) && is_object($this->view->pdf)) {
      if ($this->options['last_row'] == TRUE && ($this->countRecords + 1 >= $this->view->total_rows)) {
        return '';
      }

      $this->countRecords++;
      if ($this->countRecords == $this->view->total_rows + 1) {
        $this->countRecords = 1;
      }

      $output = '';
      if ($this->countRecords % $this->options['every_nth'] == 0) {
        $output .= '<br pagebreak="true" />';
      }
      else {
        $output .= '';
      }

      return $output;
    }
  }

  /**
   * We don't want to use advanced rendering.
   */
  function allow_advanced_render() {
    return FALSE;
  }
}
