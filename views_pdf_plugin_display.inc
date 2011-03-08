<?php

class views_pdf_plugin_display extends views_plugin_display_page {
	function get_style_type() { return 'pdf'; }
	function uses_breadcrumb() { return FALSE; }

	function render() {
		// Generall document layout
		
		// Set default code
		$this->view->pdf->SetFont('');
		
		// Add leading pages
		$path = $this->view->pdf->getTemplatePath($this->get_option('leading_template'));
		$this->view->pdf->addPdfDocument($path);
		
		// Set the default background template
		$path = $this->view->pdf->getTemplatePath($this->get_option('template'));
		$this->view->pdf->setDefaultPageTemplate($path, 'main');	
			
		// Render the items
		$this->view->style_plugin->render();

		// Add succeed pages
		$path = $this->view->pdf->getTemplatePath($this->get_option('succeed_template'));
		$this->view->pdf->addPdfDocument($path);
		
	}

	function preview() {
		return t('The PDF display does not provide a preview.');
	}
	
	
	function execute() {
		
		if($this->get_option('default_page_format') == 'custom') {
			if(preg_match('/([0-9]+)x([0-9]+)/', $this->get_option('default_page_format_custom'), $result)) {
				$format[0] = $result[1]; // width
				$format[1] = $result[2]; // height
			}
			else {
				$format = 'A4';
			}
		
		}
		else {
			$format = $this->get_option('default_page_format');
		}
				
		$orientation = $this->get_option('default_page_orientation'); // P or L
		$unit = $this->get_option('unit');
		$this->view->pdf = views_pdf_get_new_pdf_instance($orientation, $unit, $format);
		
		// Set margins: top, left, right
		$this->view->pdf->SetMargins($this->get_option('margin_left'), $this->get_option('margin_top'), $this->get_option('margin_right'), true);
		
		// Set auto page break: margin bottom:
		$this->view->pdf->SetAutoPageBreak(true, $this->get_option('margin_bottom'));
		
		
		$this->view->pdf->setDefaultFontSize($this->get_option('default_font_size'));
		$this->view->pdf->setDefaultFontFamily($this->get_option('default_font_family'));
		$this->view->pdf->setDefaultFontStyle($this->get_option('default_font_style'));
		$this->view->pdf->setDefaultTextAlign($this->get_option('default_text_align'));
		$this->view->pdf->setDefaultFontColor($this->get_option('default_font_color'));
		
		$html = $this->view->render($this->display->id);
		
		if(!empty($html)) {
			echo $html;
		}
		
		$this->view->pdf->Output($this->view->name);
		
	}

	function defaultable_sections($section = NULL) {
		if (in_array($section, array('style_options', 'style_plugin', 'row_options', 'row_plugin',))) {
			return FALSE;
		}

		$sections = parent::defaultable_sections($section);

		// Tell views our sitename_title option belongs in the title section.
		if ($section == 'title') {
			$sections[] = 'sitename_title';
		}
		elseif (!$section) {
			$sections['title'][] = 'sitename_title';
		}
		return $sections;
	}

	function option_definition() {
		$options = parent::option_definition();

		$options['displays'] = array('default' => array());

		// Overrides for standard stuff:
		$options['style_plugin']['default'] = 'pdf';
		$options['style_options']['default']  = array('mission_description' => FALSE, 'description' => '');
		$options['sitename_title']['default'] = FALSE;
		$options['row_plugin']['default'] = '';
		$options['defaults']['default']['style_plugin'] = FALSE;
		$options['defaults']['default']['style_options'] = FALSE;
		$options['defaults']['default']['row_plugin'] = FALSE;
		$options['defaults']['default']['row_options'] = FALSE;
		
		// New Options
		$options['default_page_format'] = array('default' => 'A4');
		$options['default_page_format_custom'] = array('default' => '');
		$options['default_page_orientation'] = array('default' => 'P');
		$options['unit'] = array('default' => 'mm');
		$options['margin_left'] = array('default' => '15');
		$options['margin_right'] = array('default' => '15');
		$options['margin_top'] = array('default' => '15');
		$options['margin_bottom'] = array('default' => '15');
		
		$options['leading_template'] = array('default' => '');
		$options['template'] = array('default' => '');
		$options['succeed_template'] = array('default' => '');
		
		$options['default_font_family'] = array('default' => 'helvetica');
		$options['default_font_style'] = array('default' => array());
		$options['default_font_size'] = array('default' => '11');
		$options['default_text_align'] = array('default' => 'L');
		$options['default_font_color'] = array('default' => '000000');

		return $options;
	}
	
	function options_form(&$form, &$form_state) {
		parent::options_form($form, $form_state);
		
		switch ($form_state['section']) {
			case 'pdf_page':
				$form['#title'] .= t('PDF Page Options');
				$form['default_page_format'] = array(
					'#type' => 'select',
					'#title' => t('Default Page Format'),
					'#required' => TRUE,
					'#options' => views_pdf_get_page_formats(),
					'#description' => t('This is the default page format. If you specifiy a different format in the template section, this settings will be override.'),
					'#default_value' => $this->get_option('default_page_format'),
				);
				$form['default_page_format_custom'] = array(
					'#type' => 'textfield',
					'#title' => t('Custom Page Format'),
					'#description' => t('Here you can specifiy a custom page format. The schema is "[width]x[height]".'),
					'#default_value' => $this->get_option('default_page_format_custom'),
				);
				$form['default_page_orientation'] = array(
					'#type' => 'radios',
					'#title' => t('Default Page Orientation'),
					'#required' => TRUE,
					'#options' => array('P' => t('Portrait'), 'L' => t('Landscape')),
					'#description' => t('This is the default page orientation.'),
					'#default_value' => $this->get_option('default_page_orientation'),
				);
				$form['unit'] = array(
					'#type' => 'select',
					'#title' => t('Unit'),
					'#required' => TRUE,
					'#options' => array('mm' => t('mm: Millimeter'), 'pt' => t('pt: Point'), 'cm' => t('cm: Centimeter'), 'in' => t('in: Inch')),
					'#description' => t('This is the unit for the entered unit data. If you change this option all defined units were changed, but not converted.'),
					'#default_value' => $this->get_option('unit'),
				);
				$form['margin_left'] = array(
					'#type' => 'textfield',
					'#title' => t('Margin: Left'),
					'#required' => TRUE,
					'#default_value' => $this->get_option('margin_left'),
				);
				$form['margin_right'] = array(
					'#type' => 'textfield',
					'#title' => t('Margin: Right'),
					'#required' => TRUE,
					'#default_value' => $this->get_option('margin_right'),
				);
				$form['margin_top'] = array(
					'#type' => 'textfield',
					'#title' => t('Margin: Top'),
					'#required' => TRUE,
					'#default_value' => $this->get_option('margin_top'),
				);
				$form['margin_bottom'] = array(
					'#type' => 'textfield',
					'#title' => t('Margin: Bottom'),
					'#required' => TRUE,
					'#default_value' => $this->get_option('margin_bottom'),
				);
				break;
			case 'pdf_fonts':
				$fonts = views_pdf_get_font_list();
				$font_styles = array(
					'b' => t('Bold'),
					'i' => t('Italic'),
					'u' => t('Underline'),
					'd' => t('Line through'),
					'o' => t('Overline')
				);
		
				$align = array(
					'L' => t('Left'),
					'C' => t('Center'),
					'R' => t('Right'),
					'J' => t('Justify'),
				);	
						
				$form['#title'] .= t('PDF Default Font Options');
				$form['description'] = array(
					'#prefix' => '<div class="description form-item">',
					'#suffix' => '</div>',
					'#value' => t('Here you specify a the default font settings for the document.'),
				);
				$form['default_font_size'] = array(
					'#type' => 'textfield',
					'#title' => t('Font Size'),
					'#size' => 10,
					'#default_value' => $this->get_option('default_font_size'),
				);
				$form['default_font_family'] = array(
					'#type' => 'select',
					'#title' => t('Font Family'),
					'#options' => $fonts,
					'#size' => 5,
					'#default_value' => $this->get_option('default_font_family'),
				);
				$form['default_font_style'] = array(
					'#type' => 'checkboxes',
					'#title' => t('Font Style'),
					'#options' => $font_styles,
					'#default_value' => $this->get_option('default_font_style'),
				);
				$form['default_text_align'] = array(
					'#type' => 'radios',
					'#title' => t('Text Alignment'),
					'#options' => $align,
					'#default_value' => $this->get_option('default_text_align'),
				);
				$form['default_font_color'] = array(
					'#type' => 'textfield',
					'#title' => t('Text Color (Hexadecimal)'),
					'#size' => 10,
					'#default_value' => $this->get_option('default_font_color'),
				);
		
				break;
			
			case 'pdf_template':
				$form['#title'] .= t('PDF Templates');

				$templates = array_merge(array(t(' -- None -- ')), views_pdf_get_pdf_templates());
				$form['leading_template'] = array(
					'#type' => 'select',
					'#options' => $templates,
					'#title' => t('Leading PDF Template'),
					'#required' => FALSE,
					'#description' => t('Here you specify a PDF file to be printed in front of every row.'),
					'#default_value' => $this->get_option('leading_template'),
				);
		
		
				$form['template'] = array(
					'#type' => 'select',
					'#options' => $templates,
					'#title' => t('Template PDF'),
					'#description' => t('Here you specify a PDF file on which the content is printed. The first page of this document is used for the first page, in the target document. The second page is used for the second page in the target document and so on. If the target document has more that this template file, the last page of the template will be repeated. The leading document has no effect on the order of the pages.'),
					'#default_value' => $this->get_option('template'),
				);
		
		
				$form['succeed_template'] = array(
					'#type' => 'select',
					'#options' => $templates,
					'#title' => t('Succeed PDF Template'),
					'#required' => FALSE,
					'#description' => t('Here you specify a PDF file to be printed after the main content.'),
					'#default_value' => $this->get_option('succeed_template'),
				);
		
		
				$form['template_file'] = array(
					'#type' => 'file',
					'#title' => t('Upload New Template File'),
				);				
				
				
				
				break;
		}
	}
	
	function options_summary(&$categories, &$options) {
		parent::options_summary($categories, $options);
		
		$fonts = views_pdf_get_font_list();
		
		$options['pdf_page'] = array(
			'category' => 'basic',
			'title' => t('PDF Page Settings'),
			'value' => $this->get_option('default_page_format'),
			'desc' => t('Define some PDF specific settings.'),
		);		
		
		$options['pdf_fonts'] = array(
			'category' => 'basic',
			'title' => t('PDF Fonts Settings'),
			'value' => t('!family at !size pt', array('!family' => $fonts[$this->get_option('default_font_family')], '!size' => $this->get_option('default_font_size'))),
			'desc' => t('Define some PDF specific settings.'),
		);		
		
		if($this->get_option('leading_template') != '' || $this->get_option('template') != '' || $this->get_option('succeed_template') != '') {
			$isAnyTemplate = t('Yes');
		}
		else {
			$isAnyTemplate = t('No');		
		}
		
		
		$options['pdf_template'] = array(
			'category' => 'basic',
			'title' => t('PDF Template Settings'),
			'value' => $isAnyTemplate,
			'desc' => t('Define some PDF specific settings.'),
		);		
		
		
		
	}
	
	function options_submit(&$form, &$form_state) {
		// It is very important to call the parent function here:
		parent::options_submit($form, $form_state);
		switch ($form_state['section']) {
			case 'pdf_page':
				$this->set_option('default_page_format', $form_state['values']['default_page_format']);
				$this->set_option('default_page_format_custom', $form_state['values']['default_page_format_custom']);
				$this->set_option('default_page_orientation', $form_state['values']['default_page_orientation']);
				$this->set_option('unit', $form_state['values']['unit']);
				$this->set_option('margin_left', $form_state['values']['margin_left']);
				$this->set_option('margin_right', $form_state['values']['margin_right']);
				$this->set_option('margin_top', $form_state['values']['margin_top']);
				$this->set_option('margin_bottom', $form_state['values']['margin_bottom']);
				
				break;
				
			case 'pdf_fonts': 
				$this->set_option('default_font_size', $form_state['values']['default_font_size']);
				$this->set_option('default_font_style', $form_state['values']['default_font_style']);
				$this->set_option('default_font_family', $form_state['values']['default_font_family']);
				$this->set_option('default_text_align', $form_state['values']['default_text_align']);
				$this->set_option('default_font_color', $form_state['values']['default_font_color']);
			
				break;
			
			case 'pdf_template':
				
				$this->set_option('leading_template', $form_state['values']['leading_template']);
				$this->set_option('template', $form_state['values']['template']);
				$this->set_option('succeed_template', $form_state['values']['succeed_template']);
				
				// Save new file:
				// TODO: Finde out why there is an error on ajax use with the form.
				//       The problems comes from the fact that we upload via an AJAX request. 
				//       This is not allowed, so we use a iframe, but this cause some error in
				//       combination of a json response.
				$files_path = file_directory_path();
				$template_dir = variable_get('views_pdf_template_path','views_pdf_templates');
				$dir = $files_path.'/'.$template_dir;
				if(!is_dir($dir)) {
					@mkdir($dir);
					@chmod($dir, 0775);
				}
								
				if(is_writable($dir)) {
					$template_path = file_create_path($dir);
					$file = file_save_upload('succeed_template_file', array(), $template_path);
				}
								

			
				break;
		}
	}


}
