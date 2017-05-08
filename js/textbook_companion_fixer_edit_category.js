(function($) {
	$(document).ready(function() {
		var basePath = Drupal.settings.basePath;
		//var modPath = basePath + "textbook_companion_fixer/";
		var modPath = basePath +
			"textbook_companion_fixer/ajax/edit-book-category/";
		$category_form = $("#fix-tbc-category-form");
		$(".main-subcategory-table-div").hide();

		/*********************************************/
		//$('#main-subcategory-table-'+ 1).show();
		//var main_cat_chk_value = [];
		$("input[name='ids[]']:checked").each(function() {
			main_cat_chk_value = $(this).val();
			console.log('ooo' + main_cat_chk_value);
			if (main_cat_chk_value) {
				$('#main-subcategory-table-div-id-' + main_cat_chk_value).show();
			} else {
				$('#main-subcategory-table-div-id-' + main_cat_chk_value).hide();
			}
		});

		$('.main-category-checkbox').change(function() {
			main_cat_chk_value = $(this).val();
			if (!this.checked)
				$('#main-subcategory-table-div-id-' + main_cat_chk_value).hide();
			else
				$('#main-subcategory-table-div-id-' + main_cat_chk_value).show();
		});

		$("#fix-tbc-category-form").on('click', '#btn-add', function() {
			//$('#btn-add').click(function(){
			var selectID = $(this).attr("data-cid");
			console.log(selectID);
			$('#subcats-' + selectID + ' option:selected').each(function() {
				$('#selected-subcats-' + selectID).append("<option value='" + $(this)
					.val() + "'>" + $(this).text() + "</option>");

				/**********************/
				console.log($('.main-subcategory-' + selectID).attr('data-cid'));
				var pref_id = $('.prefrence_id').val();
				var main_cat_chk_value = selectID;
				var sub_cat_select_value = $(this).val();
				$.ajax({
					url: modPath,
					type: "POST",
					data: {
						pref_id: pref_id,
						main_category: main_cat_chk_value,
						sub_category: sub_cat_select_value,
						action: "add"
					},
					dataType: "html",
					success: function(data) {
						//alert("Updated");
						console.log("My data:" + data);
						$updating.hide();
						$done.show();
						console.log("data1: " + main_cat_chk_value + " data2: " +
							sub_cat_select_value + " data3: " + pref_id);
						$done.fadeOut("slow");
						//alert("ok");
					}
				});
				/**********************/

				$(this).remove();
			});
		});
		$("#fix-tbc-category-form").on('click', '#btn-remove', function() {
			//$('#btn-remove').click(function(){
			var selectID = $(this).attr("data-cid");
			$('#selected-subcats-' + selectID + ' option:selected').each(function() {
				$('#subcats-' + selectID).append("<option value='" + $(this).val() +
					"'>" + $(this).text() + "</option>");
				var action = "delete";
				/**********************/
				var pref_id = $('.prefrence_id').val();
				var main_cat_chk_value = selectID;
				var sub_cat_select_value = $(this).val();
				$.ajax({
					url: modPath,
					type: "POST",
					data: {
						pref_id: pref_id,
						main_category: main_cat_chk_value,
						sub_category: sub_cat_select_value,
						action: action
					},
					dataType: "html",
					success: function(data) {
						//alert("Updated");
						console.log("My data:" + data);
						$updating.hide();
						$done.show();
						console.log(action + "data1: " + main_cat_chk_value +
							" data2: " + sub_cat_select_value + " data3: " + pref_id);
						$done.fadeOut("slow");
						//alert("ok");
					}
				});
				/**********************/
				$(this).remove();
			});
		});

		//$("#main_cat_checkbox").change(function() {
		$("#fix-tbc-category-form").on('change', '.main-category-checkbox',
			function() {
				var selectID = $(this).val();
				prop = $(this).prop('checked');
				if (prop) {
					$('main-subcategory-table-' + selectID).show();
				} else {
					if (confirm('Are you sure?')) {
						alert('Thanks for confirming');
						var action = "delete-main-with-ub-category";
						var pref_id = $('.prefrence_id').val();
						var main_cat_chk_value = selectID;
						console.log(action + "data1: " + main_cat_chk_value + " data2: " +
							action + " data3: " + pref_id);
						//ConfirmFunction();
						$.ajax({
							url: modPath,
							type: "POST",
							data: {
								pref_id: pref_id,
								main_category: main_cat_chk_value,
								action: action
							},
							dataType: "html",
							success: function(data) {
								//alert("Updated");
								location.reload();
								$('main-subcategory-table-' + selectID).hide();
								console.log("My data:" + data);
								$updating.hide();
								$done.show();
								console.log(action + "data1: " + main_cat_chk_value + " data2: " +
									action + " data3: " + pref_id);
								$done.fadeOut("slow");
								//alert("ok");
							}
						});
					} else {
						alert('You have not confirmed the action');
						location.reload();
					}
				}
			});
		/**********************************************************************/
	});
})(jQuery);

function ConfirmFunction() {
	confirm("Are you sure?");
}
