var app = app || {};

(function(app) {
	// News
	app.NewsItCompanyItemView = Backbone.Marionette.ItemView.extend({
		template : '#rss-item-template',
	});

	app.NewsItCompanyCompositeView = Backbone.Marionette.CompositeView.extend({
		template: '#NewsItCompany-composite-template',
		childView : app.NewsItCompanyItemView,
		childViewContainer : 'span',
	});

	app.NewsItCompanyLayoutView = Backbone.Marionette.LayoutView.extend({
		template: '#NewsItCompany-layout-template',

		regions : {
			listRegion : '#NewsItCompany-lists',
		},

		onRender : function(){
			var NewsItCompanyCollection = new app.NewsItCompanyCollection();
			this.listenTo(NewsItCompanyCollection , 'reset', this.showList, this);
			NewsItCompanyCollection.fetch({reset : true});
		},

		showList : function(NewsItCompanyCollection){
			this.listRegion.show( new app.NewsItCompanyCompositeView({
				collection : NewsItCompanyCollection
			}));
		},

	});

})(app);
