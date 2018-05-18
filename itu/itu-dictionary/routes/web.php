<?php

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

Route::get('/', function () {
    return view('auth/login');
});

Auth::routes();
Route::get('/logout', 'Auth\LogoutController@logout');
Route::post('/register', 'Auth\RegisterController@create');

Route::group(['middleware' => ['auth']], function(){

    Route::get('/home', function (){
        return view('home');
    });

    Route::get('/course', function (){
        return view('course');
    });

    Route::get('/lesson', function (){
        return view('lesson');
    });

    Route::get('/tests', function (){
        return view('tests');
    });

    Route::get('/password_reset', function (){
        return view('auth/passwords/reset');
    });

    Route::get('{course}/{lesson}/millionaire', 'MillionaireController@index');

    Route::get('{course}/{lesson}/pexeso', 'PexesoController@index');

});
