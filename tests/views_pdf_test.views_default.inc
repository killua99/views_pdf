<?php

/**
 * @file
 * Tests views.
 */

/**
 * Implements hook_views_default_views().
 */
function views_pdf_test_views_default_views() {
  $view = new view;
  $view->name = 'test_views_pdf_groupby_save';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->version = 7;
  $view->disabled = FALSE;

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';

  $views[$view->name] = $view;

  return $views;
}
