<?php
function textbook_companion_fixer_edit_book_proposal_all()
{
	//get the book count
	$result = db_query("SELECT COUNT( pe.book ) AS book_count FROM textbook_companion_preference pe LEFT JOIN textbook_companion_proposal po ON pe.proposal_id = po.id WHERE po.proposal_status =3 AND pe.approval_status =1 AND pe.category>0");
	$row = $result->fetchObject();
	$book_count = $row->book_count;
	$i = 1;
	/* get preference */
	$preference_q = db_query("SELECT pe.id as pref_id, pe.book as book, pe.author as author,pe.category as existing_category, pe.publisher as publisher, pe.year as year, pe.id as pe_id, po.approval_date as approval_date
  FROM {textbook_companion_preference} pe LEFT JOIN {textbook_companion_proposal} po ON pe.proposal_id = po.id
  WHERE po.proposal_status = 3 AND pe.approval_status = 1 AND pe.category>0 ORDER BY pe.book ASC");
	while ($preference_data = $preference_q->fetchObject())
	{
		$proposal_rows[] = array(
			$i,
			"{$preference_data->book} <br> <em>by {$preference_data->author}</em>",
			_textbook_companion_fixer_list_of_category($preference_data->existing_category),
			_tbc_fixer_list_of_new_category($preference_data->pref_id),
			l('Edit', 'textbook_companion_fixer/category_edit/' . $preference_data->pref_id)
		);
		$i++;
	} //$proposal_data = $proposal_q->fetchObject()
	/* check if there are any pending proposals */
	if (!$proposal_rows)
	{
		drupal_set_message(t('There are no proposals.'), 'status');
		return '';
	} //!$proposal_rows
	$output .= "Book count with category: " . $book_count;
	$proposal_header = array(
		'No.',
		'Title of the Book',
		'Existing Category',
		'New Category',
		'Action'
	);
	$output .= theme('table', array(
		'header' => $proposal_header,
		'rows' => $proposal_rows
	));
	return $output;
}
/******************************************************************************/
/**************************** CATEGORY EDIT FORM ******************************/
/******************************************************************************/
function textbook_companion_fixer_category_edit_form($form, &$form_state)
{
	/* get current proposal */
	$preference_id = arg(2);
	$query = db_select('textbook_companion_preference');
	$query->fields('textbook_companion_preference');
	$query->condition('id', $preference_id);
	$preference_q = $query->execute();
	$preference_data = $preference_q->fetchObject();
	if (!$preference_data)
	{
		drupal_set_message(t('Invalid book selected. Please try again.'), 'error');
		drupal_goto('manage_proposal/category');
		return;
	} //!$preference_data
	$form["wrapper"] = array(
		"#type" => "fieldset",
		"#title" => "Edit the existing book category to new category",
		"#prefix" => "<div id='fix-tbc-category-form'>",
		"#suffix" => "</div>"
	);
	$form["wrapper"]['book'] = array(
		'#type' => 'item',
		'#title' => t('Title of the book'),
		'#markup' => $preference_data->book
	);
	$form["wrapper"]['author'] = array(
		'#type' => 'item',
		'#title' => t('Author Name'),
		'#markup' => $preference_data->author
	);
	$form["wrapper"]['isbn'] = array(
		'#type' => 'item',
		'#title' => t('ISBN No'),
		'#markup' => $preference_data->isbn
	);
	$form["wrapper"]['publisher'] = array(
		'#type' => 'item',
		'#title' => t('Publisher & Place'),
		'#markup' => $preference_data->publisher
	);
	$form["wrapper"]['edition'] = array(
		'#type' => 'item',
		'#title' => t('Edition'),
		'#markup' => $preference_data->edition
	);
	$form["wrapper"]['year'] = array(
		'#type' => 'item',
		'#title' => t('Year of pulication'),
		'#markup' => $preference_data->year
	);
	$form["wrapper"]['pref_id'] = array(
		'#markup' => '<input class="prefrence_id" type="hidden" name="pref_id" value="' . $preference_data->id . '">'
	);
	$form["wrapper"]['main_category'] = array(
		"#markup" => _textbook_companion_fixer_list_of_category_checkboxes()
	);
	$form["wrapper"]['back'] = array(
		'#markup' => l(t('Back'), 'textbook_companion_fixer/edit_book_category')
	);
	return $form;
}
function textbook_companion_fixer_edit_book_category_ajax($item, $key)
{
	global $user;
	$data = "";
	$item = arg(2);
	$main_category = $_POST['main_category'];
	$sub_category = $_POST['sub_category'];
	if ($item == "edit-book-category")
	{
		if ($_POST['action'] == "add")
		{
			$query_in1 = "
                INSERT INTO {textbook_companion_book_main_subcategories}
                (pref_id, main_category, sub_category)
                VALUES
                (:pref_id, :main_category, :subcategory)
                ";
			$args_in1 = array(
				':pref_id' => $_POST['pref_id'],
				':main_category' => $main_category,
				':subcategory' => $sub_category
			);
			$result_in1 = db_query($query_in1, $args_in1);
			/* sending email */
		$email_to = $user->mail;
		$from = variable_get('textbook_companion_from_email', '');
		$bcc = variable_get('textbook_companion_fixer_bcc_emails', '');
		$cc = variable_get('textbook_companion_fixer_cc_emails', '');
		$params['category_updated']['pref_id'] = $_POST['pref_id'];
		$params['category_updated']['main_category'] = $main_category;
		$params['category_updated']['sub_category'] = $sub_category;
		$params['category_updated']['user_id'] = $user->uid;
		$params['category_updated']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('textbook_companion_fixer', 'new_category_updated', $email_to, language_default(), $params, $from, TRUE)) {
			$data .= 'Error sending email message.';
		}
		} //$_POST['action'] == "add"
		elseif ($_POST['action'] == "delete")
		{
			$query_del1 = "DELETE FROM textbook_companion_book_main_subcategories
WHERE pref_id= :pref_id AND main_category= :main_category AND sub_category = :subcategory
                ";
			$args_del1 = array(
				':pref_id' => $_POST['pref_id'],
				':main_category' => $_POST['main_category'],
				':subcategory' => $_POST['sub_category']
			);
			$result_del1 = db_query($query_del1, $args_del1);
			/* sending email */
		$email_to = $user->mail;
		$from = variable_get('textbook_companion_from_email', '');
		$bcc = variable_get('textbook_companion_fixer_bcc_emails', '');
		$cc = variable_get('textbook_companion_fixer_cc_emails', '');
		$params['subcategory_deleted']['pref_id'] = $_POST['pref_id'];
		$params['subcategory_deleted']['main_category'] = $main_category;
		$params['subcategory_deleted']['sub_category'] = $sub_category;
		$params['subcategory_deleted']['user_id'] = $user->uid;
		$params['subcategory_deleted']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('textbook_companion_fixer', 'new_subcategory_deleted', $email_to, language_default(), $params, $from, TRUE)) {
			$data .= 'Error sending email message.';
		}
		} //$_POST['action'] == "delete"
		elseif ($_POST['action'] == "delete-main-with-ub-category")
		{
			$query_del2 = "DELETE FROM textbook_companion_book_main_subcategories
WHERE pref_id= :pref_id AND main_category= :main_category
                ";
			$args_del2 = array(
				':pref_id' => $_POST['pref_id'],
				':main_category' => $_POST['main_category']
			);
			$result_del2 = db_query($query_del2, $args_del2);
					/* sending email */
		$email_to = $user->mail;
		$from = variable_get('textbook_companion_from_email', '');
		$bcc = variable_get('textbook_companion_fixer_bcc_emails', '');
		$cc = variable_get('textbook_companion_fixer_cc_emails', '');
		$params['maincategory_deleted']['pref_id'] = $_POST['pref_id'];
		$params['maincategory_deleted']['main_category'] = $main_category;
		$params['maincategory_deleted']['user_id'] = $user->uid;
		$params['maincategory_deleted']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('textbook_companion_fixer', 'new_maincategory_deleted', $email_to, language_default(), $params, $from, TRUE)) {
			$data .= 'Error sending email message.';
		}
		} //$_POST['action'] == "delete-main-with-ub-category"
		else
		{
			$data = "Not Updated";
		}
	} //$item == "edit-book-category"
	else
	{
		$data = "Not Updated";
	}
	echo $data;
	exit();
}
function _tbc_fixer_list_of_category($category_id = NULL)
{
	$category[0] = "Please select";
	if ($category_id == NULL)
	{
		$query = db_select('list_of_category');
		$query->fields('list_of_category');
		$query->orderBy('id', 'ASC');
		$category_list = $query->execute();
	} //$category_id == NULL
	else
	{
		$query = db_select('list_of_category');
		$query->fields('list_of_category');
		$query->condition('category_id', $category_id);
		$query->orderBy('id', 'ASC');
		$category_list = $query->execute();
	}
	while ($category_list_data = $category_list->fetchObject())
	{
		$category[$category_list_data->category_id] = $category_list_data->category_name;
	} //$category_list_data = $category_list->fetchObject()
	return $category;
}
function _textbook_companion_fixer_list_of_category($category_id)
{
	$category .= "";
	$query = db_select('list_of_category');
	$query->fields('list_of_category');
	$query->condition('id', $category_id);
	$category_list = $query->execute();
	while ($category_list_data = $category_list->fetchObject())
	{
		$category = $category_list_data->category_name;
	} //$category_list_data = $category_list->fetchObject()
	return $category;
}
function _tbc_fixer_list_of_new_category($pref_id)
{
	$category = "";
	$main_category_query = "
                SELECT  distinct(maincategory)
                FROM list_of_category loc 
                LEFT JOIN textbook_companion_book_main_subcategories tcbms ON loc.category_id = tcbms.main_category
                LEFT JOIN  list_of_subcategory los ON los.subcategory_id = tcbms.sub_category
                WHERE tcbms.pref_id = :pref_id ORDER BY loc.category_id
                ";
	$args = array(
		':pref_id' => $pref_id
	);
	$maincategory_list = db_query($main_category_query, $args);
	$category .= "<ol>";
	while ($category_list_data = $maincategory_list->fetchObject())
	{
		$category .= "<li>$category_list_data->maincategory</li>";
		$sub_category_query = "
                SELECT DISTINCT (los.subcategory)
                FROM list_of_category loc 
                LEFT JOIN textbook_companion_book_main_subcategories tcbms ON loc.category_id = tcbms.main_category
                LEFT JOIN  list_of_subcategory los ON los.subcategory_id = tcbms.sub_category
                WHERE tcbms.pref_id = :pref_id and maincategory = :maincategoryvalue
                ";
		$sub_args = array(
			':pref_id' => $pref_id,
			':maincategoryvalue' => $category_list_data->maincategory
		);
		$sub_category_list = db_query($sub_category_query, $sub_args);
		while ($sub_category_list_data = $sub_category_list->fetchObject())
		{
			$category .= "<ul><li>$sub_category_list_data->subcategory</li></ul>";
		} //$sub_category_list_data = $sub_category_list->fetchObject()
	} //$category_list_data = $maincategory_list->fetchObject()
	$category .= "</ol>";
	return $category;
}
function _tbc_fixer_list_of_ext_new_category($pref_id, $category_id)
{
	$category = "";
	$query = "
                SELECT  maincategory, los.subcategory as subcategory 
                FROM list_of_category loc 
                LEFT JOIN textbook_companion_book_main_subcategories tcbms 
                ON loc.category_id = tcbms.main_category 
                LEFT JOIN  list_of_subcategory los ON los.subcategory_id = tcbms.sub_category
                WHERE tcbms.pref_id = :pref_id AND loc.category_id = :category_id
                ";
	$args = array(
		':pref_id' => $pref_id,
		':category_id' => $category_id
	);
	$category_list = db_query($query, $args);
	while ($category_list_data = $category_list->fetchObject())
	{
		$category .= $category_list_data->maincategory;
	} //$category_list_data = $category_list->fetchObject()
	return $category;
}
function _textbook_companion_fixer_list_of_category_checkboxes()
{
	$pref_id = arg(2);
	$query = db_select('list_of_category');
	$query->fields('list_of_category');
	//$query->fields(array('category_id','main_category'));
	$query->orderBy('category_id', 'ASC');
	$category_list = $query->execute();
	while ($category_list_data = $category_list->fetchObject())
	{
		$categoryname = $category_list_data->maincategory;
		if ($categoryname != null || strlen($categoryname) != 0)
		{
			//$category[$category_list_data->category_id] = $category_list_data->main_category;
			$existing_category = _tbc_fixer_list_of_ext_new_category($pref_id, $category_list_data->category_id);
			$existing_subcategory = _tbc_fixer_list_of_ext_new_subcategory($pref_id, $category_list_data->category_id);
			$checked = $existing_category ? 'checked="checked"' : '';
			$category .= "<input id='main_cat_checkbox-" . $category_list_data->category_id . "' type='checkbox' name='ids[]' value='" . $category_list_data->category_id . "' class='main-category-checkbox' " . $checked . " >" . $category_list_data->maincategory . "<br>
		<div id ='main-subcategory-table-div-id-" . $category_list_data->category_id . "' class='main-subcategory-table-div'><table id='main-subcategory-table-" . $category_list_data->category_id . "' class='main-subcategory-table'>
  <tr>
    <th>Available sub categories</th>
    <th>Selected sub categories</th>
  </tr>
  <tr>
    <td rowspan='3'><select id='subcats-" . $category_list_data->category_id . "' name='subcats' class='main-subcategory' size='10'  multiple name='subcat' data-cid='" . $category_list_data->category_id . "'>" . _textbook_companion_fixer_list_of_subcategory($pref_id, $category_list_data->category_id) . "</select>
    <a href='JavaScript:void(0);' id='btn-add' class='select-main-subcategory-" . $category_list_data->category_id . "' data-cid='" . $category_list_data->category_id . "'>Add &raquo;</a>
    </td>
    <td rowspan='3'><a href='JavaScript:void(0);' id='btn-remove' class='select-main-subcategory-" . $category_list_data->category_id . "'data-cid='" . $category_list_data->category_id . "'>&laquo; Remove</a>
    <select id='selected-subcats-" . $category_list_data->category_id . "' name='subcats' class='select-main-subcategory-list' size='10'  multiple name='subcat' data-cid='" . $category_list_data->category_id . "'>" . _tbc_fixer_list_of_ext_new_subcategory($pref_id, $category_list_data->category_id) . "</select>
    </td>
  </tr>
</table></div>
		<hr>";
		} //$category_list_data = $category_list->fetchObject()
	} //$category_list_data = $category_list->fetchObject()
	return $category;
}
function _textbook_companion_fixer_list_of_subcategory($pref_id, $category_id)
{
	$query = "
                SELECT los.subcategory_id as subcat_id,los.subcategory as sub_category 
                FROM list_of_subcategory los WHERE los.maincategory_id= :category_id AND los.subcategory_id 
                NOT IN (SELECT los.subcategory_id as sub_id from list_of_subcategory los 
                LEFT OUTER JOIN textbook_companion_book_main_subcategories tcbms 
                ON los.subcategory_id=tcbms.sub_category WHERE tcbms.pref_id= :pref_id ORDER BY sub_id)
                ";
	$args = array(
		':pref_id' => $pref_id,
		':category_id' => $category_id
	);
	$subcategory_list = db_query($query, $args);
	while ($subcategory_list_data = $subcategory_list->fetchObject())
	{
		$subcategory .= "<option value='" . $subcategory_list_data->subcat_id . "' data-name='" . $subcategory_list_data->sub_category . "' data-mcid='" . $category_id . "'>" . $subcategory_list_data->sub_category . "</option>";
	} //$subcategory_list_data = $subcategory_list->fetchObject()
	return $subcategory;
}
function _tbc_fixer_list_of_ext_new_subcategory($pref_id, $category_id)
{
	$subcategory = "";
	$query = "
                SELECT DISTINCT (los.subcategory), maincategory, los.subcategory as subcategory, 
                los.subcategory_id as subcat_id FROM list_of_category loc 
                LEFT JOIN textbook_companion_book_main_subcategories tcbms ON loc.category_id = tcbms.main_category
                LEFT JOIN  list_of_subcategory los ON los.subcategory_id = tcbms.sub_category
                WHERE tcbms.pref_id = :pref_id AND loc.category_id = :category_id
              ";
	$args = array(
		':pref_id' => $pref_id,
		':category_id' => $category_id
	);
	$subcategory_list = db_query($query, $args);
	while ($subcategory_list_data = $subcategory_list->fetchObject())
	{
		$subcategory .= "<option value='" . $subcategory_list_data->subcat_id . "' data-name='" . $subcategory_list_data->subcategory . "' data-mcid='" . $category_id . "'>" . $subcategory_list_data->subcategory . "</option>";
	} //$category_list_data = $category_list->fetchObject()
	return $subcategory;
}
