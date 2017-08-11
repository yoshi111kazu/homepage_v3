var app = app || {};

(function(app) {
	// News
	app.NewsHealthItemView = Backbone.Marionette.ItemView.extend({
		//tagName : 'li',

		template : '#rss-item-template',

	});

	app.NewsHealthCompositeView = Backbone.Marionette.CompositeView.extend({
		template: '#NewsHealth-composite-template',

		childView : app.NewsHealthItemView,

		childViewContainer : 'span',

	});

	app.NewsHealthLayoutView = Backbone.Marionette.LayoutView.extend({
		template: '#NewsHealth-layout-template',

		regions : {
			listRegion : '#NewsHealth-lists',
		},

		onRender : function(){
			var NewsHealthCollection = new app.NewsHealthCollection();
			this.listenTo(NewsHealthCollection , 'reset', this.showList, this);
			NewsHealthCollection.fetch({reset : true});
		},

		showList : function(NewsHealthCollection){
			this.listRegion.show( new app.NewsHealthCompositeView({
				collection : NewsHealthCollection
			}));
		},

	});

})(app);
