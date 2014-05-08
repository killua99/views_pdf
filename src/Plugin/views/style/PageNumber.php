<?php

/**
 * @file
 * Contains \Drupal\views_pdf\Plugin\views\style\PageNumber.
 */

// We can't use name space in views 7.x-x.x
// namespace Drupal\views_pdf\Plugin\views\style;

/**
 * Class that holds the functionality for the page number in a PDF display.
 *
 * @ingroup views_field_handler
 */
class PageNumber extends views_handler_field {

  /**
   * This method  is used to query data. In our case we want that no data is queried.
   */
  function query() {
    // Override parent::query() and don't alter query.
    $this->field_alias = 'pdf_page_number_' . $this->position;
  }

  /**
   * This method adds a page number to the display, if it is a PDF display.
   *
   * Therefore the PDF class is used.
   */
  function render($values) {

    if (isset($this->view->pdf) && is_object($this->view->pdf)) {
      return $this->view->pdf->getPage();
    }
    else {
      return '';
    }

  }

  /**
   * We don't want to use advanced rendering.
   */
  function allow_advanced_render() {
    return FALSE;
  }
}
