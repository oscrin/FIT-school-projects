## [ITU] WebApp for vocabulary learning

#### Before using:
Configure (hidden) `.env` file! Use your local MySQL database server with new database!

#### Basic commands (perform in project root):
* `php artisan migrate:fresh` - Refresh MySQL database tables
* `php artisan db:seed` - Insert new testing rows to database tables
* `php artisan serve` - Starts local server on port 8000 (`http://127.0.0.1:8000/`)

## TODO List
* Find some super-duper template and divide it logical into blades -  Honza
  * `~/resources/views/layouts/`
    * Header
    * Menu
    * Footer
    * Main `app.blade.php` layout that includes header, menu and footer (using e.g. `@include('layouts.menu')`), and set place, where content of particular pages should be placed (using e.g. `@yield('title')`)
  * `~/resources/views/`
    * Particular page is a blade, that `@extends('layouts.app')`
    * content of particular pages (in `@section`, that will be yielded into main app)
* Make middleware (+ help Honza) - Mirec
* Edit / add some new DB tables + seeds in particular seeders (e.g. `~/database/seeds/WordsSeeder.php`) - Viktor
