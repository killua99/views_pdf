<?php

/**
 * @file
 * Contains \Drupal\views_pdf\ViewsPdfSettings.
 */

namespace Drupal\views_pdf;

/**
 * Base class for views pdf.
 */
class ViewsPdfBase {

  /**
   * List with the available fonts.
   * @var array
   */
  protected static $fontList = NULL;

  /**
   * Clean font list.
   * @var array
   */
  protected static $fontListClean = NULL;

  /**
   * List with the uploaded templates.
   * @var array
   */
  protected static $templateList = NULL;

  /**
   * Hyphenate.
   * @var array
   */
  protected static $hyphenatePatterns = NULL;

  protected $defaultFontStyle            = '';
  protected $defaultFontFamily           = 'Helvetica';
  protected $defaultFontSize             = '11';
  protected $defaultTextAlign            = 'L';
  protected $defaultFontColor            = '000000';
  protected $defaultPageTemplateFiles    = array();
  protected $mainContentPageNumber       = 0;
  protected $rowContentPageNumber        = 0;
  protected $defaultOrientation          = 'P';
  protected $defaultFormat               = 'A4';
  protected $addNewPageBeforeNextContent = FALSE;
  protected $elements                    = array();
  protected $headerFooterData            = array();
  protected $viewsHeader                = '';
  protected $view                        = NULL;
  protected $viewsFooter                = '';
  protected $headerFooterOptions         = array();
  protected $lastWritingPage             = 1;
  protected $lastWritingPositions;

  protected static $defaultFontList = array(
    'almohanad'              => 'AlMohanad',
    'arialunicid0'           => 'ArialUnicodeMS',
    'courier'                => 'Courier',
    'courierb'               => 'Courier Bold',
    'courierbi'              => 'Courier Bold Italic',
    'courieri'               => 'Courier Italic',
    'dejavusans'             => 'DejaVuSans',
    'dejavusansb'            => 'DejaVuSans-Bold',
    'dejavusansbi'           => 'DejaVuSans-BoldOblique',
    'dejavusansi'            => 'DejaVuSans-Oblique',
    'dejavusanscondensed'    => 'DejaVuSansCondensed',
    'dejavusanscondensedb'   => 'DejaVuSansCondensed-Bold',
    'dejavusanscondensedbi'  => 'DejaVuSansCondensed-BoldOblique',
    'dejavusanscondensedi'   => 'DejaVuSansCondensed-Oblique',
    'dejavusansmono'         => 'DejaVuSansMono',
    'dejavusansmonob'        => 'DejaVuSansMono-Bold',
    'dejavusansmonobi'       => 'DejaVuSansMono-BoldOblique',
    'dejavusansmonoi'        => 'DejaVuSansMono-Oblique',
    'dejavuserif'            => 'DejaVuSerif',
    'dejavuserifb'           => 'DejaVuSerif-Bold',
    'dejavuserifbi'          => 'DejaVuSerif-BoldItalic',
    'dejavuserifi'           => 'DejaVuSerif-Italic',
    'dejavuserifcondensed'   => 'DejaVuSerifCondensed',
    'dejavuserifcondensedb'  => 'DejaVuSerifCondensed-Bold',
    'dejavuserifcondensedbi' => 'DejaVuSerifCondensed-BoldItalic',
    'dejavuserifcondensedi'  => 'DejaVuSerifCondensed-Italic',
    'freemono'               => 'FreeMono',
    'freemonob'              => 'FreeMonoBold',
    'freemonobi'             => 'FreeMonoBoldOblique',
    'freemonoi'              => 'FreeMonoOblique',
    'freesans'               => 'FreeSans',
    'freesansb'              => 'FreeSansBold',
    'freesansbi'             => 'FreeSansBoldOblique',
    'freesansi'              => 'FreeSansOblique',
    'freeserif'              => 'FreeSerif',
    'freeserifb'             => 'FreeSerifBold',
    'freeserifbi'            => 'FreeSerifBoldItalic',
    'freeserifi'             => 'FreeSerifItalic',
    'hysmyeongjostdmedium'   => 'HYSMyeongJoStd-Medium-Acro',
    'helvetica'              => 'Helvetica',
    'helveticab'             => 'Helvetica Bold',
    'helveticabi'            => 'Helvetica Bold Italic',
    'helveticai'             => 'Helvetica Italic',
    'kozgopromedium'         => 'KozGoPro-Medium-Acro',
    'kozminproregular'       => 'KozMinPro-Regular-Acro',
    'msungstdlight'          => 'MSungStd-Light-Acro',
    'stsongstdlight'         => 'STSongStd-Light-Acro',
    'symbol'                 => 'Symbol',
    'times'                  => 'Times New Roman',
    'timesb'                 => 'Times New Roman Bold',
    'timesbi'                => 'Times New Roman Bold Italic',
    'timesi'                 => 'Times New Roman Italic',
    'zapfdingbats'           => 'Zapf Dingbats',
    'zarbold'                => 'ZarBold',
  );

  /**
   * This method returns a list of current uploaded files.
   */
  public static function getAvailableTemplates() {
    if (self::$templateList != NULL) {
      return self::$templateList;
    }

    $files_path     = drupal_realpath('public://');
    $template_dir   = variable_get('views_pdf_template_path', 'views_pdf_templates');
    $dir            = $files_path . '/' . $template_dir;
    $templatesFiles = file_scan_directory($dir, '/.pdf$/', array('nomask' => '/(\.\.?|CVS)$/'), 1);

    $templates = array();

    foreach ($templatesFiles as $file) {
      $templates[$file->filename] = $file->name;
    }

    self::$templateList = $templates;

    return $templates;

  }

  /**
   * This method returns a list of available fonts.
   */
  public static function getAvailableFonts() {
    if ($this->fontList != NULL) {
      return $this->fontList;
    }

    // Get all pdf files with the font list: K_PATH_FONTS
    $fonts = file_scan_directory(K_PATH_FONTS, '/.php$/', array(
      'nomask'  => '/(\.\.?|CVS)$/',
      'recurse' => FALSE,
      ), 1);
    $cache = cache_get('views_pdf_cached_fonts');

    $cached_font_mapping = NULL;

    if (is_object($cache)) {
      $cached_font_mapping = $cache->data;
    }

    if (is_array($cached_font_mapping)) {
      $font_mapping = array_merge($this->defaultFontList, $cached_font_mapping);
    }
    else {
      $font_mapping = $this->defaultFontList;
    }

    foreach ($fonts as $font) {
      $name = $this->getFontNameByFileName($font->uri);
      if (isset($name)) {
        $font_mapping[$font->name] = $name;
      }
    }

    asort($font_mapping);

    cache_set('views_pdf_cached_fonts', $font_mapping);

    // Remove all fonts without name.
    foreach ($font_mapping as $key => $font) {
      if (empty($font)) {
        unset($font_mapping[$key]);
      }

    }

    $this->fontList = $font_mapping;

    return $font_mapping;
  }

  /**
   * This method returns a cleaned up version of the font list.
   */
  public static function getAvailableFontsCleanList() {
    if ($this->fontListClean !== NULL) {
      return $this->fontListClean;
    }

    $clean = $this->getAvailableFonts();

    foreach ($clean as $key => $font) {

      // Unset bold, italic, italic/bold fonts.
      unset($clean[($key . 'b')]);
      unset($clean[($key . 'bi')]);
      unset($clean[($key . 'i')]);

    }

    $this->$fontListClean = $clean;

    return $clean;
  }


}
