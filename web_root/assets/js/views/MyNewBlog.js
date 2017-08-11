var app = app || {};

(function(app) {
	// News
	app.MyNewBlogItemView = Backbone.Marionette.ItemView.extend({
		//tagName : 'li',
		template : '#MyNewBlog-item-template',
	});

	app.MyNewBlogCompositeView = Backbone.Marionette.CompositeView.extend({
		template: '#MyNewBlog-composite-template',

		childView : app.MyNewBlogItemView,

		childViewContainer : 'span',

	});

	app.MyNewBlogLayoutView = Backbone.Marionette.LayoutView.extend({
		template: '#MyNewBlog-layout-template',

		regions : {
			listRegion : '#MyNewBlog-lists',
		},

		onRender : function(){
			var MyNewBlogCollection = new app.MyNewBlogCollection();
			this.listenTo(MyNewBlogCollection , 'reset', this.showList, this);
			MyNewBlogCollection.fetch({reset : true});
		},

		showList : function(MyNewBlogCollection){
			this.listRegion.show( new app.MyNewBlogCompositeView({
				collection : MyNewBlogCollection
			}));
		},

	});

})(app);
