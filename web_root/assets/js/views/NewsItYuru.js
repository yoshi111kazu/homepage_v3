var app = app || {};

(function(app) {
	// News
	app.NewsItYuruItemView = Backbone.Marionette.ItemView.extend({
		//tagName : 'li',

		template : '#rss-item-template',

	});

	app.NewsItYuruCompositeView = Backbone.Marionette.CompositeView.extend({
		template: '#NewsItYuru-composite-template',

		childView : app.NewsItYuruItemView,

		childViewContainer : 'span',

	});

	app.NewsItYuruLayoutView = Backbone.Marionette.LayoutView.extend({
		template: '#NewsItYuru-layout-template',

		regions : {
			listRegion : '#NewsItYuru-lists',
		},

		onRender : function(){
			var NewsItYuruCollection = new app.NewsItYuruCollection();
			this.listenTo(NewsItYuruCollection , 'reset', this.showList, this);
			NewsItYuruCollection.fetch({reset : true});
		},

		showList : function(NewsItYuruCollection){
			this.listRegion.show( new app.NewsItYuruCompositeView({
				collection : NewsItYuruCollection
			}));
		},

	});

})(app);
