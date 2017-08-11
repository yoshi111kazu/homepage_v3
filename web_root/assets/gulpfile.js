var gulp = require("gulp");  
var concat = require("gulp-concat");
var uglify = require('gulp-uglify');
//var jshint = require('gulp-jshint');
//var eslint = require('gulp-eslint');
var cssmin = require('gulp-cssmin');
var rename = require('gulp-rename');
var sass = require('gulp-sass');
var autoprefixer = require('gulp-autoprefixer');
var minifyHTML = require('gulp-minify-html');
var imagemin = require('gulp-imagemin');
var cache = require('gulp-cached');
var pngquant = require('imagemin-pngquant');
var runSequence = require('run-sequence');

gulp.task('default', function() {
  console.log('--------- no task ----------');
});

gulp.task('full', ['all', 'image']);
gulp.task('all', ['html', 'css', 'sass', 'js']);
gulp.task('html', ['html-min']);
gulp.task( 'css', function() { return runSequence( 'css-merge', 'css-min' ); });
gulp.task( 'sass', function() { return runSequence( 'sass-compile', 'sass-min' ); });
gulp.task( 'js', function() { return runSequence( ['js-merge-01', 'js-merge-02'], 'js-min' ); });
gulp.task('image', ['image-min']);

//html
gulp.task("html-min", function() {
  return gulp.src('../index_edit.html')
    .pipe(minifyHTML({ empty: true }))
    .pipe(rename("index.html"))
    .pipe(gulp.dest('../'))
    ;
});

//css
gulp.task("css-merge", function() {
  var files = [
   './css/ishino.css',
   './css/font-awesome.min.css',
   './css/style.css'
  ];
  return gulp.src(files)
    .pipe(concat('bundle.css'))
    .pipe(gulp.dest('./css/'))
    ;
});
gulp.task('css-min', function() {
  gulp.src('./css/bootstrap_grid.css')
    .pipe(cssmin())
    .pipe(rename({ extname: '.min.css' }))
    .pipe(gulp.dest('./css/'))
  gulp.src('./css/bundle.css')
    .pipe(cssmin())
    .pipe(rename({ extname: '.min.css' }))
    .pipe(gulp.dest('./css/'))
  return 1;
});

gulp.task('sass-compile', function(){
  return gulp.src('./css/sass/html5up.scss')
    .pipe(sass.sync().on('error', sass.logError))
    .pipe(gulp.dest('./css/'))
});
gulp.task('sass-min', function() {
  return gulp.src('./css/html5up.css')
    .pipe(cssmin())
    .pipe(rename({ extname: '.min.css' }))
    .pipe(gulp.dest('./css/'))
});

//js
gulp.task("js-merge-01", function() {
  var files = [
   './node_modules/jquery/dist/jquery.min.js',
   './js/lib/skel.min.js',
   './js/lib/skel-layout.min.js',
   './js/lib/html5up-util.js',
   './js/lib/html5up.js'
  ];
  return gulp.src(files)
    .pipe(concat('bundle_01.js'))
    //.pipe(jshint())
    //.pipe(jshint.reporter('jshint-stylish'))
    //.pipe( eslint() )
    //.pipe( eslint.format() )
    //.pipe( eslint.failAfterError() )
    .pipe(gulp.dest('./js/'))
    ;
});

gulp.task("js-merge-02", function() {
  var files = [
   './node_modules/bootstrap/dist/js/bootstrap.min.js',
   './node_modules/json2/lib/JSON2/static/json2.js',
   './node_modules/underscore/underscore-min.js',
   './node_modules/backbone/backbone-min.js',
   './node_modules/backbone.marionette/lib/backbone.marionette.min.js',
   './js/models/rss-model.js',
   './js/collections/collection.js',
   './js/views/Top.js',
   './js/views/NewsMusic.js',
   './js/views/NewsItIt.js',
   './js/views/NewsItProgram.js',
   './js/views/NewsItInfra.js',
   './js/views/NewsItPosting.js',
   './js/views/NewsItCompany.js',
   './js/views/NewsItYuru.js',
   './js/views/NewsItLink.js',
   './js/views/NewsHealth.js',
   './js/views/NewsCar.js',
   './js/views/NewsGame.js',
   './js/views/Blog.js',
   './js/views/Profile.js',
   './js/views/MyNewBlog.js',
   './js/app.js',
   './js/ishino.js'
  ];
  return gulp.src(files)
    .pipe(concat('bundle_02.js'))
    //.pipe(jshint())
    //.pipe(jshint.reporter('jshint-stylish'))
    //.pipe( eslint() )
    //.pipe( eslint.format() )
    //.pipe( eslint.failAfterError() )
    .pipe(gulp.dest('./js/'))
    ;
});

gulp.task('js-min', function() {
  gulp.src('./js/bundle_01.js')
    .pipe(uglify({preserveComments: 'some'}))
    .pipe(rename({ extname: '.min.js' }))
    .pipe(gulp.dest('./js/'))
  gulp.src('./js/bundle_02.js')
    .pipe(uglify({preserveComments: 'some'}))
    .pipe(rename({ extname: '.min.js' }))
    .pipe(gulp.dest('./js/'))
  return 1;
});

//image
gulp.task("image-min", function() {
  return gulp.src('./image/src/*.+(jpg|jpeg|png|gif|svg)')
    .pipe(cache('imagemining'))
    .pipe(imagemin({
      progressive: true,
      svgoPlugins: [
        { removeViewBox: false },
        { cleanupIDs: false }
      ],
      use: [pngquant()]
    }))
    .pipe(rename({prefix: '_'}))
    .pipe(gulp.dest('./image/'))
    ;
});
