<?php

/**
 * @file
 * Needed to support test.
 */

/**
 * Implements hook_permission().
 */
function views_pdf_test_permission() {
	return array(
		'views_pdf_test test permission' => array(
			'title' => t('Test permission'),
			'description' => t('views_pdf_test test permission'),
		),
	);
}

