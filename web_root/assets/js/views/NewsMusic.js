var app = app || {};

(function(app) {
	// Music
	app.NewsMusicOverseaItemView = Backbone.Marionette.ItemView.extend({
		template : '#rss-item-template',
	});

	app.NewsMusicOverseaCompositeView = Backbone.Marionette.CompositeView.extend({
		template: '#NewsMusicOversea-composite-template',
		childView : app.NewsMusicOverseaItemView,
		childViewContainer : 'span',
	});

	app.NewsMusicOverseaLayoutView = Backbone.Marionette.LayoutView.extend({
		template: '#NewsMusicOversea-layout-template',

		regions : {
			listRegion : '#NewsMusicOversea-lists',
		},

		onRender : function(){
			var musicCollection = new app.NewsMusicOverseaCollection();
			this.listenTo(musicCollection , 'reset', this.showList, this);
			musicCollection.fetch({reset : true});
		},

		showList : function(musicCollection){
			this.listRegion.show( new app.NewsMusicOverseaCompositeView({
				collection : musicCollection
			}));
		},
	});

	app.NewsMusicItemItemView = Backbone.Marionette.ItemView.extend({
		template : '#rss-item-template',
	});

	app.NewsMusicItemCompositeView = Backbone.Marionette.CompositeView.extend({
		template: '#NewsMusicItem-composite-template',
		childView : app.NewsMusicItemItemView,
		childViewContainer : 'span',
	});

	app.NewsMusicItemLayoutView = Backbone.Marionette.LayoutView.extend({
		template: '#NewsMusicItem-layout-template',

		regions : {
			listRegion : '#NewsMusicItem-lists',
		},

		onRender : function(){
			var musicCollection = new app.NewsMusicItemCollection();
			this.listenTo(musicCollection , 'reset', this.showList, this);
			musicCollection.fetch({reset : true});
		},

		showList : function(musicCollection){
			this.listRegion.show( new app.NewsMusicItemCompositeView({
				collection : musicCollection
			}));
		},
	});

})(app);
