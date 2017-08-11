var app = app || {};

(function(app) {
	// News
	app.NewsItInfraItemView = Backbone.Marionette.ItemView.extend({
		//tagName : 'li',

		template : '#rss-item-template',

	});

	app.NewsItInfraCompositeView = Backbone.Marionette.CompositeView.extend({
		template: '#NewsItInfra-composite-template',

		childView : app.NewsItInfraItemView,

		childViewContainer : 'span',

	});

	app.NewsItInfraLayoutView = Backbone.Marionette.LayoutView.extend({
		template: '#NewsItInfra-layout-template',

		regions : {
			listRegion : '#NewsItInfra-lists',
		},

		onRender : function(){
			var NewsItInfraCollection = new app.NewsItInfraCollection();
			this.listenTo(NewsItInfraCollection , 'reset', this.showList, this);
			NewsItInfraCollection.fetch({reset : true});
		},

		showList : function(NewsItInfraCollection){
			this.listRegion.show( new app.NewsItInfraCompositeView({
				collection : NewsItInfraCollection
			}));
		},

	});

})(app);
