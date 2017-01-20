<?php
// $Id$
function textbook_companion_fixer_settings_form($form,&$form_state)
{
	$form['to_emails'] = array(
		'#type' => 'textfield',
		'#title' => t('(To) Notification emails'),
		'#description' => t('A comma separated list of email addresses to receive notifications emails'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => variable_get('textbook_companion_fixer_to_emails', '')
	);
	$form['bcc_emails'] = array(
		'#type' => 'textfield',
		'#title' => t('(BCc) Notification emails'),
		'#description' => t('Specify emails id for BCC option of mail system with comma separated'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => variable_get('textbook_companion_fixer_bcc_emails', '')
	);
	$form['cc_emails'] = array(
		'#type' => 'textfield',
		'#title' => t('(Cc) Notification emails'),
		'#description' => t('Specify emails id for CC option of mail system with comma separated'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => variable_get('textbook_companion_fixer_cc_emails', '')
	);
	$form['from_email'] = array(
		'#type' => 'textfield',
		'#title' => t('(From) Outgoing from email address'),
		'#description' => t('Email address to be display in the from field of all outgoing messages'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => variable_get('textbook_companion_fixer_from_email', '')
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	return $form;
}
function textbook_companion_fixer_settings_form_validate($form, &$form_state)
{
	return;
}
function textbook_companion_fixer_settings_form_submit($form, &$form_state)
{
	variable_set('textbook_companion_fixer_to_emails', $form_state['values']['to_emails']);
	variable_set('textbook_companion_fixer_bcc_emails', $form_state['values']['bcc_emails']);
	variable_set('textbook_companion_fixer_cc_emails', $form_state['values']['cc_emails']);
	variable_set('textbook_companion_fixer_from_email', $form_state['values']['from_email']);
}
