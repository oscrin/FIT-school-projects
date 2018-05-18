@extends('layouts.app')

@section('title', 'Domov')

@section('content')
    <!-- Content Header (Page header) -->
    <section class="content-header">
        <h1>

            Hra Milionář
            <small>Lekce {{ $lesson }}</small>
        </h1>
        <ol class="breadcrumb">
            <li><a href="#"><i class="fa fa-dashboard"></i>Domov</a></li>
            <li><a href="#">Kurz {{ $course }}</a></li>
            <li><a href="#">Lekce {{ $lesson }}</a></li>
            <li><a href="#">Výuka</a></li>
            <li class="active">Hra Milionář</li>
        </ol>
    </section>

    <!-- Main content -->
    <section class="content">
    
    <div class="box">
        <div class="box-header with-border">
            <h3 id="word_title" class="box-title">Apple</h3>
        </div>
        <div class="box-body">
            <div>
                <div class="text-center">
                    <h3>Co v češtině znamená slovo:</h3>
                </div>
                <div class="text-center">
                    <h2 id="word"><strong>"{{ $words[0]->en }}"</strong></h2>
                </div>
            </div>
            <br>
            <div class="odpovedi">
                <button type="button" id="option1" class="btn btn-default btn-block btn-lg">{{ $words[0]->w1 }}</button>
                <button type="button" id="option2" class="btn btn-default btn-block btn-lg">{{ $words[0]->w2 }}</button>
                <button type="button" id="option3" class="btn btn-default btn-block btn-lg">{{ $words[0]->w3 }}</button>
                <button type="button" id="option4" class="btn btn-default btn-block btn-lg">{{ $words[0]->w4 }}</button>
            </div>
        </div>
        <div class="box-footer">
            Slovíčko <span id="word_num"></span> z <span id="word_nums"></span>
        </div>
    </div>

    <div class="row">
        <div class="col-md-3 back">
            <button type="button" id="back" class="btn btn-block btn-warning">Předchozí slovíčko</button>
        </div>
        <div class="col-md-6">
        </div>
        <div class="col-md-3 next">
            <button type="button" id="next" class="btn btn-block btn-success">Další slovíčko</button>
        </div>
    </div>

    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
    <script>
        @php
            $escapers = array("\\",     "/",   "\"",  "\n",  "\r",  "\t", "\x08", "\x0c");
            $replacements = array("\\\\", "\\/", "\\\"", "\\n", "\\r", "\\t",  "\\f",  "\\b");
            $escaped_words = str_replace($escapers, $replacements, json_encode($words));
        @endphp

        var words = $.parseJSON('{!! $escaped_words !!}');

        var idx = 0;

        $(document).ready( function(){
            $("#word_num").text(idx+1);
            $("#word_nums").text(words.length);

            $(".odpovedi button").click( function () {
                if(words[idx].cs == $(this).text()){
                    $(this).toggleClass("btn-default").toggleClass("btn-success");
                }
                else{
                    $(this).toggleClass("btn-default").toggleClass("btn-danger");
                }
            });

            $(".next button").click( function () {
                idx++;

                if(idx < words.length)
                {
                    $("#word_num").text(idx+1);

                    $("#word strong").text("\""+words[idx].en+"\"");
                    $("#word_title").text(words[idx].en);

                    $("#option1").text(words[idx].w1);
                    $("#option2").text(words[idx].w2);
                    $("#option3").text(words[idx].w3);
                    $("#option4").text(words[idx].w4);

                    $(".odpovedi button").removeClass("btn-success").removeClass("btn-danger").addClass("btn-default");
                }
                else {
                    idx--;
                }
            });

            $(".back button").click( function () {
                idx--;

                if(idx >= 0)
                {
                    $("#word_num").text(idx+1);

                    $("#word strong").text("\""+words[idx].en+"\"");
                    $("#word_title").text(words[idx].en);

                    $("#option1").text(words[idx].w1);
                    $("#option2").text(words[idx].w2);
                    $("#option3").text(words[idx].w3);
                    $("#option4").text(words[idx].w4);

                    //$(".odpovedi button").removeClass("btn-success").removeClass("btn-danger").addClass("btn-default");
                }
                else {
                    idx++;
                }

            });
        });
    </script>


    </section>
    <!-- /.content -->
@endsection