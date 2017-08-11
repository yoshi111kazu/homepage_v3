var app = app || {};

(function(app) {
	// News
	app.NewsItProgramItemView = Backbone.Marionette.ItemView.extend({
		//tagName : 'li',

		template : '#rss-item-template',

	});

	app.NewsItProgramCompositeView = Backbone.Marionette.CompositeView.extend({
		template: '#NewsItProgram-composite-template',

		childView : app.NewsItProgramItemView,

		childViewContainer : 'span',

	});

	app.NewsItProgramLayoutView = Backbone.Marionette.LayoutView.extend({
		template: '#NewsItProgram-layout-template',

		regions : {
			listRegion : '#NewsItProgram-lists',
		},

		onRender : function(){
			var NewsItProgramCollection = new app.NewsItProgramCollection();
			this.listenTo(NewsItProgramCollection , 'reset', this.showList, this);
			NewsItProgramCollection.fetch({reset : true});
		},

		showList : function(NewsItProgramCollection){
			this.listRegion.show( new app.NewsItProgramCompositeView({
				collection : NewsItProgramCollection
			}));
		},

	});

})(app);
