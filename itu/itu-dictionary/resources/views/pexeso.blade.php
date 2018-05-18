@extends('layouts.app')

@section('title', 'Pexeso')

@section('content')
    <!-- Content Header (Page header) -->
    <section class="content-header">
        <h1>
            Pexeso
            <small>Výuka slovíček</small>
        </h1>
        <ol class="breadcrumb">
            <li><a href="{{ url('/home') }}"><i class="fa fa-dashboard"></i>Domov</a></li>
            <li><a href="#">Kurz {{ $course }}</a></li>
            <li><a href="#">Lekce {{ $lesson }}</a></li>
            <li>Výuka</li>
            <li class="active">Pexeso</li>
        </ol>
    </section>

    <!-- Main content -->
    <section class="content">
        <div class="row">
            <div class="col-sm-9">
                <div class="box">
                    <div class="box-body">
                        <div id="cards" class="row">
                            @foreach($words as $word)
                            <div class="col-sm-2">
                                <div id="card{{ $word->id }}en" data-word="{{ $word->id }}" class="box box-solid box-default" onclick="show_card('{{ $word->id }}en')">
                                    <div class="box-body bg-gray" style="height: 80px; padding-top: 20px;">
                                        <h5 style="display: none;" class="text-center">{{ $word->en }}</h5>
                                    </div>
                                </div>
                            </div>
                            <div class="col-sm-2">
                                <div id="card{{ $word->id }}cs" data-word="{{ $word->id }}" class="box box-solid box-default" onclick="show_card('{{ $word->id }}cs')">
                                    <div class="box-body bg-gray" style="height: 80px; padding-top: 20px;">
                                        <h5 style="display: none;" class="text-center">{{ $word->cs }}</h5>
                                    </div>
                                </div>
                            </div>
                            @endforeach
                        </div>
                    </div>
                </div>
            </div>
            <div class="col-sm-3">
                <div class="small-box bg-yellow">
                    <div class="inner">
                        <h3 id="score">0</h3>

                        <p>Skóre</p>
                    </div>
                    <div class="icon">
                        <i class="ion ion-stats-bars"></i>
                    </div>
                </div>
            </div>
        </div>
    </section>
    <!-- /.content -->

    <script type="text/javascript">
        var shown = -1;
        var score = 0;

        function show_card(id) {
            if(shown == id){
                $('#card'+id+' h5').fadeOut(100, function () {
                    $('#card'+id+' div').toggleClass('bg-info').toggleClass('bg-gray');
                });
                shown = -1;
            }
            else if(shown == -1){
                shown = id;
                $('#card'+id+' h5').fadeIn(100, function(){
                    $('#card'+id+' div').toggleClass('bg-gray').toggleClass('bg-info');
                });
            }
            else{
                var card1 = $('#card'+shown).data('word');
                var card2 = $('#card'+id).data('word');

                $('#card'+id+' h5').fadeIn(100, function(){
                    $('#card'+id+' div').toggleClass('bg-gray').toggleClass('bg-info');
                });

                if(card1 == card2){
                    $('#card'+shown+' div').toggleClass('bg-info').toggleClass('bg-success');
                    $('#card'+shown).toggleClass('box-default').toggleClass('box-success');

                    $('#card'+id+' div').toggleClass('bg-info').toggleClass('bg-success');
                    $('#card'+id).toggleClass('box-default').toggleClass('box-success');

                    $('#card'+shown).attr('onclick','');
                    $('#card'+id).attr('onclick','');
                    shown = -1;
                    score += 5;
                    $('#score').text(score);
                }
                else {
                    $('#card'+id+' div').toggleClass('bg-info').toggleClass('bg-danger');
                    $('#card'+shown+' div').toggleClass('bg-info').toggleClass('bg-danger');

                    score -= 1;
                    $('#score').text(score);

                    setTimeout(function () {
                        $('#card'+id+' div').toggleClass('bg-danger').toggleClass('bg-gray');
                        $('#card'+shown+' div').toggleClass('bg-danger').toggleClass('bg-gray');
                        $('#card'+id+' h5').fadeOut(100);
                        $('#card'+shown+' h5').fadeOut(100);
                        shown = -1;
                    }, 1000);
                }
            }

        }
    </script>
@endsection