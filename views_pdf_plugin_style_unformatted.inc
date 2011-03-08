<?php


class views_pdf_plugin_style_unformatted extends views_plugin_style
{
	function render() {
		if ($this->uses_row_plugin() && empty($this->row_plugin)) {
			vpr('views_plugin_style_default: Missing row plugin');
			return;
		}
		
		// Group the rows according to the grouping field, if specified.
		//$sets = $this->render_grouping($this->view->result, $this->options['grouping']);

		// Render each group separately and concatenate.  Plugins may override this
		// method if they wish some other way of handling grouping.
		$output = '';
		/*foreach ($sets as $title => $records) {

			// Header of style 
			
			if ($this->uses_row_plugin()) {
				$this->row_plugin->render($records);
			}
			
			// Print content as it is with the html function
			else {
			
			}

			// Footer of style	

		}*/
		
		//echo 'style-render';
		//echo '<br />';
		//print_r($this->view->result);
		
		/*foreach($this->view->result as $records) {*/
		$this->view->numberOfRecords = count($this->view->result);
			if ($this->uses_row_plugin()) {
				$this->row_plugin->render($this->view->result);
			}		
		//}

		//unset($this->view->row_index);
		return $output;
	}

}
