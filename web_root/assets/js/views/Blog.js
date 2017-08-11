var app = app || {};

(function(app) {
	app.BlogItemView = Backbone.Marionette.ItemView.extend({
		template: '#Blog-template',
	});
})(app);
