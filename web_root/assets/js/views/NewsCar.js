var app = app || {};

(function(app) {
	// News
	app.NewsCarItemView = Backbone.Marionette.ItemView.extend({
		//tagName : 'li',

		template : '#rss-item-template',

	});

	app.NewsCarCompositeView = Backbone.Marionette.CompositeView.extend({
		template: '#NewsCar-composite-template',

		childView : app.NewsCarItemView,

		childViewContainer : 'span',

	});

	app.NewsCarLayoutView = Backbone.Marionette.LayoutView.extend({
		template: '#NewsCar-layout-template',

		regions : {
			listRegion : '#NewsCar-lists',
		},

		onRender : function(){
			var carCollection = new app.NewsCarCollection();
			this.listenTo(carCollection , 'reset', this.showList, this);
			carCollection.fetch({reset : true});
		},

		showList : function(carCollection){
			this.listRegion.show( new app.NewsCarCompositeView({
				collection : carCollection
			}));
		},

	});

})(app);
