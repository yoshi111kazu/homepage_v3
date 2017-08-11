var app = app || {};

(function(app) {
	// News
	app.NewsGameItemView = Backbone.Marionette.ItemView.extend({
		//tagName : 'li',

		template : '#rss-item-template',

	});

	app.NewsGameCompositeView = Backbone.Marionette.CompositeView.extend({
		template: '#NewsGame-composite-template',

		childView : app.NewsGameItemView,

		childViewContainer : 'span',

	});

	app.NewsGameLayoutView = Backbone.Marionette.LayoutView.extend({
		template: '#NewsGame-layout-template',

		regions : {
			listRegion : '#NewsGame-lists',
		},

		onRender : function(){
			var NewsGameCollection = new app.NewsGameCollection();
			this.listenTo(NewsGameCollection , 'reset', this.showList, this);
			NewsGameCollection.fetch({reset : true});
		},

		showList : function(NewsGameCollection){
			this.listRegion.show( new app.NewsGameCompositeView({
				collection : NewsGameCollection
			}));
		},

	});

})(app);
