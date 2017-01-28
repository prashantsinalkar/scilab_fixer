(function($) {
	$(document).ready(function() {
		var basePath = Drupal.settings.basePath;
		var modPath = basePath + "textbook_companion_fixer/ajax/edit-book-category/";
		$category_form = $("#fix-tbc-category-form");
		$main_category1 = $(".main-category-1");
		$main_category2 = $(".main-category-2");
		$main_category3 = $(".main-category-3");
		$main_category4 = $(".main-category-4");
		$main_category5 = $(".main-category-5");
		$main_category6 = $(".main-category-6");
		$main_category7 = $(".main-category-7");
		$(".main-subcategory-1").hide();
		$(".main-subcategory-2").hide();
		$(".main-subcategory-3").hide();
		$(".main-subcategory-4").hide();
		$(".main-subcategory-5").hide();
		$(".main-subcategory-6").hide();
		$(".main-subcategory-7").hide();
		prop = $('.main-category-chk-1').prop('checked');
		console.log(prop);
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
		$category_form.submit(function(e) {
			e.preventDefault();
		});

	});
})(jQuery);
