<?php

/**
 * @file
 * Contains \Drupal\views_pdf\Plugin\views\display\ViewsPdfPage.
 */

namespace Drupal\views_pdf\Plugin\views\display;

use Drupal\views\Plugin\views\display\Page;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\State\StateInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * The plugin that handles a Views PDF Page.
 *
 * @ingroup views_display_plugins
 *
 * @ViewsDisplay(
 *   id = "views_pdf_page",
 *   title = @Translation("PDF Page"),
 *   help = @Translation("Display the view as a PDF page, with a URL and menu links."),
 *   uses_menu_links = TRUE,
 *   uses_route = TRUE,
 *   contextual_links_locations = {"pdf page"},
 *   theme = "views_view",
 *   admin = @Translation("PDF Page")
 * )
 */
class ViewsPdfPage extends Page {

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('router.route_provider'),
      $container->get('state'),
      $container->get('entity.manager')->getStorage('menu')
    );
  }

  /**
   * Sets the current page views render array.
   *
   * @param array $element
   *   (optional) A render array. If not specified the previous element is
   *   returned.
   *
   * @return array
   *   The page render array.
   */
  public static function &setPageRenderArray(array &$element = NULL) {
    if (isset($element)) {
      static::$pageRenderArray = &$element;
    }

    return static::$pageRenderArray;
  }

  /**
   * Gets the current views page render array.
   *
   * @return array
   *   The page render array.
   */
  public static function &getPageRenderArray() {
    return static::$pageRenderArray;
  }

  /**
   * {@inheritdoc}
   */
  protected function defineOptions() {
    $options = parent::defineOptions();

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public static function buildBasicRenderable($view_id, $display_id, array $args = [], Route $route = NULL) {
    $build = parent::buildBasicRenderable($view_id, $display_id, $args);

    if ($route) {
      $build['#view_id'] = $route->getDefault('view_id');
      $build['#view_display_plugin_id'] = $route->getOption('_view_display_plugin_id');
      $build['#view_display_show_admin_links'] = $route->getOption('_view_display_show_admin_links');
    }
    else {
      throw new \BadFunctionCallException('Missing route parameters.');
    }

    return $build;
  }

  /**
   * {@inheritdoc}
   */
  public function execute() {
    parent::execute();

    // And now render the view.
    $render = $this->view->render();

    return $render;
  }

  /**
   * {@inheritdoc}
   */
  public function optionsSummary(&$categories, &$options) {
    parent::optionsSummary($categories, $options);

  }

  /**
   * {@inheritdoc}
   */
  public function buildOptionsForm(&$form, FormStateInterface $form_state) {
    parent::buildOptionsForm($form, $form_state);

  }

  /**
   * {@inheritdoc}
   */
  public function validateOptionsForm(&$form, FormStateInterface $form_state) {
    parent::validateOptionsForm($form, $form_state);


  }

  /**
   * {@inheritdoc}
   */
  public function submitOptionsForm(&$form, FormStateInterface $form_state) {
    parent::submitOptionsForm($form, $form_state);


  }


  /**
   * {@inheritdoc}
   */
  public function validate() {
    $errors = parent::validate();

    return $errors;
  }

  /**
   * {@inheritdoc}
   */
  public function getArgumentText() {
    return array(
      'filter value not present' => $this->t('When the filter value is <em>NOT</em> in the URL'),
      'filter value present' => $this->t('When the filter value <em>IS</em> in the URL or a default is provided'),
      'description' => $this->t('The contextual filter values are provided by the URL.'),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getPagerText() {
    return array(
      'items per page title' => $this->t('Items per page'),
      'items per page description' => $this->t('The number of items to display per page. Enter 0 for no limit.')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function calculateDependencies() {
    $dependencies = parent::calculateDependencies();

    return $dependencies;
  }

}
