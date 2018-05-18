@extends('layouts.app')

@section('title', 'Domov')

@section('content')
<!-- Content Header (Page header) -->
<section class="content-header">
    <h1>
        Domů
        <small>Přehled</small>
    </h1>
    <ol class="breadcrumb">
        <li><a href="#"><i class="fa fa-dashboard"></i>Domů</a></li>
        <li class="active"><a href="#">Přehled</a></li>
    </ol>
</section>

<!-- Main content -->
<section class="content">

    <div class="row">
        <div class="col-md-8">
            <div class="box box-info">
                <div class="box-header with-border">
                    <h3 class="box-title">Poslední aktivita</h3>
                </div>
                <div class="box-body">

                    <ul class="todo-list ui-sortable">
                <li>
                  <!-- drag handle -->
                  <span class="">
                        <i class="fa fa-calendar-check-o"></i>
                  </span>
                  <!-- todo text -->
                  <span class="text">Vytvořena <a href="{{ url('/lesson') }}">Lekce 2</a>.</span>
                  <!-- Emphasis label -->
                  <small class="label label-danger"><i class="fa fa-clock-o"></i> 2 mins</small>
                  <!-- General tools such as edit or delete-->
                  <div class="tools">
                    <i class="fa fa-trash-o"></i>
                  </div>
                </li>
                <li>
                <span class="">
                <i class="fa fa-calendar-check-o"></i>
          </span>
                  <span class="text">Ve hře Milionář pro <a href="{{ url('/lesson') }}">Lekci 1</a> jste získali 7 bodů.</span>
                  <small class="label label-warning"><i class="fa fa-clock-o"></i> 1 day</small>
                  <div class="tools">
                    <i class="fa fa-trash-o"></i>
                  </div>
                </li>
                <li>
                <span class="">
                <i class="fa fa-calendar-check-o"></i>
          </span>
                  <span class="text">Ve hře Pexeso pro <a href="{{ url('/lesson') }}">Lekci 1</a> jste získali 12 bodů.</span>
                  <small class="label label-info"><i class="fa fa-clock-o"></i> 3 days</small>
                  <div class="tools">
                    <i class="fa fa-trash-o"></i>
                  </div>
                </li>
                <li>
                <span class="">
                <i class="fa fa-calendar-check-o"></i>
          </span>
                  <span class="text">Do <a href="{{ url('/lesson') }}"> Lekce 1</a> bylo přidáno 62 slovíček.</a></span>
                  <small class="label label-success"><i class="fa fa-clock-o"></i> 1 week</small>
                  <div class="tools">
                    <i class="fa fa-trash-o"></i>
                  </div>
                </li>
                <li>
                <span class="">
                <i class="fa fa-calendar-check-o"></i>
          </span>
                  <span class="text">Vytvořena <a href="{{ url('/lesson') }}">Lekce 1</a>.</span>
                  <small class="label label-primary"><i class="fa fa-clock-o"></i> 2 weeks</small>
                  <div class="tools">
                    <i class="fa fa-trash-o"></i>
                  </div>
                </li>
                <li>
                <span class="">
                <i class="fa fa-calendar-check-o"></i>
          </span>
                  <span class="text">Vytvořen <a href="{{ url('/lesson') }}">Kurz 1</a>.</span>
                  <small class="label label-default"><i class="fa fa-clock-o"></i> 1 month</small>
                  <div class="tools">
                    <i class="fa fa-trash-o"></i>
                  </div>
                </li>
              </ul>

                </div>
            </div>
        </div>

        <div class="col-md-4">
            <div class="small-box bg-red">
                <div class="inner">
                    
                    <span class="d-inline"> <h3>Tvé skóre: 27</h3> </span>

                    <p>Máš na to... Ready?</p>
                </div>
                <div class="icon">
                    <i class="ion ion-stats-bars"></i>
                </div>
                <a href="#" class="small-box-footer">
                    Aktuální výsledky <i style="padding-left:3px;" class="fa fa-arrow-circle-right"></i>
                </a>
            </div>

            <div class="info-box">
                <span class="info-box-icon bg-green"><i class="fa fa-flag-o"></i></span>

                <div class="info-box-content">
                    <span class="info-box-text"><a href="{{ url('/course') }}">Kurzy</a></span>
                    <span class="info-box-number">Celkem: <strong>1</strong></span>
                    <form action=""  class="text-right">
                        <button type="button" class="btn btn-success" data-toggle="modal" data-target="#vytvoritKurz">Vytvořit kurz</button>
                    </form>
                </div>
            </div>

            <div class="info-box">
                <span class="info-box-icon bg-yellow"><i class="fa fa-files-o"></i></span>

                <div class="info-box-content">
                    <span class="info-box-text"><a href="{{ url('/lesson') }}">Lekce</a></span>
                    <span class="info-box-number">Celkem: <strong>2</strong></span>
                    <form action=""  class="text-right">
                    <button type="button" class="btn btn-warning" data-toggle="modal" data-target="#vytvoritLekci">Vytvořit lekci</button>
                    </form>
                </div>
            </div>

            <!-- /.info-box-content -->
        </div>

        </div>

    <br>

    <div class="alert alert-info alert-dismissible" style="opacity: 0.8;">
                <button type="button" class="close" data-dismiss="alert" aria-hidden="true">×</button>
                <h4><i class="icon fa fa-info"></i> Nový Tip!</h4>
                Přidej nový kurz nebo se přihlaž do kurzu, který je vedený tvým lektorem. Usnadní ti to výuku nových slovíček a budeš si tak jistý, že ses na každou hodinu naučil vše potřebné.
              </div>
    <!-- /.box -->
</section>
<!-- /.content -->
@endsection