var app = app || {};

(function(app) {
	// News
	app.NewsItPostingItemView = Backbone.Marionette.ItemView.extend({
		template : '#rss-item-template',
	});

	app.NewsItPostingCompositeView = Backbone.Marionette.CompositeView.extend({
		template: '#NewsItPosting-composite-template',
		childView : app.NewsItPostingItemView,
		childViewContainer : 'span',
	});

	app.NewsItPostingLayoutView = Backbone.Marionette.LayoutView.extend({
		template: '#NewsItPosting-layout-template',

		regions : {
			listRegion : '#NewsItPosting-lists',
		},

		onRender : function(){
			var NewsItPostingCollection = new app.NewsItPostingCollection();
			this.listenTo(NewsItPostingCollection , 'reset', this.showList, this);
			NewsItPostingCollection.fetch({reset : true});
		},

		showList : function(NewsItPostingCollection){
			this.listRegion.show( new app.NewsItPostingCompositeView({
				collection : NewsItPostingCollection
			}));
		},

	});

})(app);
