@extends('layouts.app')

@section('title', 'Lekce')

@section('content')
    <!-- Content Header (Page header) -->
    @php 
        $lekce = 1;
        $word_num = 1;
        //echo $lekce;

        if (isset($_GET['lekce'])){
            $lekce = $_GET['lekce'];
        }
        if (isset($_GET['en']) && (isset($_GET['cs']) )){
            $cs = $_GET['cs'];
            $en = $_GET['en'];
            $id = DB::table('words')->insertGetId(
                ['cs' => "$cs", 'en' => "$en"]
            );
            $id = DB::table('lesson_word')->insertGetId(
                ['lesson_id' => "$lekce", 'word_id' => "$id"]
            );
        }
        if (isset($_GET['odebrat'])){
            $odebrat = $_GET['odebrat'];
            DB::table('lesson_word')->where('word_id', '=', $odebrat)->delete();
        }
    @endphp
    
    <section class="content-header">
        <h1>
            Přehled lekcí
            <small>Lekce {{$lekce}}</small>
        </h1>
        <ol class="breadcrumb">
            <li><a href="#"><i class="fa fa-dashboard"></i>Domů</a></li>
            <li><a href="#">Přehled lekcí</a></li>
            <li class="active">Lekce {{$lekce}}</li>
        </ol>
    </section>

    <!-- Main content -->
    <section class="content">

       
        @php 
        $words = DB::table('words')
        ->join('lesson_word', function ($join) use ($lekce){
            $join->on('words.id', '=', 'lesson_word.word_id')
                 ->where('lesson_word.lesson_id', '=', $lekce);
        })
        ->get();
        $slovicka = DB::table('words')->get();
        @endphp



        <div class="box">
            <div class="box-header">
                <div class="col-md-2 "><h4><strong>Lekce {{$lekce}}</strong></h4></div>
                <div class="col-md-8"></div>
                <div class="col-md-2">
                    <div style="margin-top: 5px;">
                        <button type="button" class="btn btn-block btn-success btn-sm">Změnit název lekce</button>
                    </div>
                </div>
            </div>
            <!-- /.box-header -->
            <div class="box-body no-padding">
              <table class="table table-condensed">
                <tbody><tr>
                  <th style="width: 30px"></th>
                  <th style="width: 50px">#</th>
                  <th style="width: 620px">Anglické slovíčko</th>
                  <th style="width: 500px">Český překlad</th>
                  <th style="width: 70px" class="text-center">Změnit</th>
                  <th style="width: 30px"></th>
                </tr>
               <!--
                <tr>
                    <td></td>
                  <td>1.</td>
                  <td>
                    Česky
                  </td>
                  <td>
                    Anglicky
                  </td>
                  <td><button class="btn btn-block btn-info btn-xs">Upravit</button></td>
                  <td></td>
            -->
            <form>
            @foreach ($words as $word)
                <tr>
                <td></td>
                    <td>{{$word_num++}}.</td>
                    <td>{{$word->en}}</td>
                    <td>{{$word->cs}}</td>
                    <td><button name="odebrat"  value="{{$word->word_id}}" class="btn btn-block btn-info btn-xs">Odebrat z lekce</button></td>
                    <td></td>
                </tr>
            @endforeach
            <input type="hidden" name="lekce" value="{{$lekce}}" >
            </form>
                </tr>
                
              </tbody></table>
            </div>
            <!-- /.box-body -->
            <div class="box-footer">
                Celkem {{$word_num - 1}} slovíček.
              </div>
        </div>

         <!-- 
        <h2>Přidat nové slovíčko</h2>
        <select class="chosen" style="width:500px;">
        @foreach ($slovicka as $word)
            <option>{{$word->cs}}</option>
        @endforeach
        </select>
        -->

        <div class="box box-success">
        <div class="box-header with-border">
          <h3 class="box-title">Přidat slovíčko</h3>

          <div class="box-tools pull-right">
            <button type="button" class="btn btn-box-tool" data-widget="collapse"><i class="fa fa-minus"></i>
            </button>
          </div>
          <!-- /.box-tools -->
        </div>
        <!-- /.box-header -->
        <div class="box-body">
                <form>
                <div class="row">
                <div class="col-xs-2">
                    <input type="text" placeholder="cz slovíčko" name="cs" value="" required>
                </div>
                <div class="col-xs-2">
                    <input type="text" placeholder="en slovíčko" name="en" value="" required>
                </div>
                <div class="col-xs-4">
                    <input type="hidden" name="lekce" value="{{$lekce}}" >
                    <button id="" class="btn btn-success btn-xs">Vložit slovíčko</button>
                </div>
                <div class="col-xs-4">
                </div>
              </div>
                </form>
            </div>
            <!-- /.box-body -->
          </div>
        
        <!-- old table
        <table style="width:100%">
            <tr>
                <th>anglické slovíčko</th>
                <th>české slovíčko</th> 
            </tr>
            @foreach ($words as $word)
                <tr>
                    <td>{{$word->en}}</td>
                    <td>{{$word->cs}}</td>
                </tr>
            @endforeach
        </table>
        -->
    </section>
    <!-- /.content -->
@endsection
