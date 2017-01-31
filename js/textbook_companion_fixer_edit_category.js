(function($) {
	$(document).ready(function() {
		var basePath = Drupal.settings.basePath;
		//var modPath = basePath + "textbook_companion_fixer/";
		var modPath = basePath + "textbook_companion_fixer/ajax/edit-book-category/";
		$category_form = $("#fix-tbc-category-form");
		$(".main-subcategory-1").hide();
		$(".main-subcategory-2").hide();
		$(".main-subcategory-3").hide();
		$(".main-subcategory-4").hide();
		$(".main-subcategory-5").hide();
		$(".main-subcategory-6").hide();
		$(".main-subcategory-7").hide();
		prop = $('.main-category-chk-1').prop('checked');
		$('.main-category-chk-1').change(function() {
			if (!this.checked) {
				$('.main-subcategory-1').hide();
				$('.main-subcategory-1').prop('selectedIndex', 0);
			} else {
				$('.main-subcategory-1').show();
			}
		});
		$('.main-category-chk-2').change(function() {
			if (!this.checked) {
				$('.main-subcategory-2').hide();
				$('.main-subcategory-2').prop('selectedIndex', 0);
			} else {
				$('.main-subcategory-2').show();
			}
		});
		$('.main-category-chk-3').change(function() {
			if (!this.checked) {
				$('.main-subcategory-3').hide();
				$('.main-subcategory-3').prop('selectedIndex', 0);
			} else {
				$('.main-subcategory-3').show();
			}
		});
		$('.main-category-chk-4').change(function() {
			if (!this.checked) {
				$('.main-subcategory-4').hide();
				$('.main-subcategory-4').prop('selectedIndex', 0);
			} else {
				$('.main-subcategory-4').show();
			}
		});
		$('.main-category-chk-5').change(function() {
			if (!this.checked) {
				$('.main-subcategory-5').hide();
				$('.main-subcategory-5').prop('selectedIndex', 0);
			} else {
				$('.main-subcategory-5').show();
			}
		});
		$('.main-category-chk-6').change(function() {
			if (!this.checked) {
				$('.main-subcategory-6').hide();
				$('.main-subcategory-6').prop('selectedIndex', 0);
			} else {
				$('.main-subcategory-6').show();
			}
		});
		$('.main-category-chk-7').change(function() {
			if (!this.checked) {
				$('.main-subcategory-7').hide();
				$('.main-subcategory-7').prop('selectedIndex', 0);
			} else {
				$('.main-subcategory-7').show();
			}
		});
		//edit category form submit
		$("#submit-button-category").click(function(e) {
			var main_cat_chk_value = [];
			$("input[name='ids[]']:checked").each(function (){
				main_cat_chk_value.push(parseInt($(this).val()));
			});
				console.log(modPath);
				$.ajax({
					url: modPath,
					type: "POST",
					data: {
						pref_id: pref_id,
						main_category: main_cat_chk_value,
						subcategory: ""
					},
					dataType: "html",
					success: function(data) {
						$updating.hide();
						$done.show();
						console.log("data: +" + main_cat_chk_value);
						$done.fadeOut("slow");
						//alert("ok");
					}
				});
			e.preventDefault();
		});

	});
})(jQuery);
