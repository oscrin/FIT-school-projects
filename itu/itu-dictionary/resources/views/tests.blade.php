@extends('layouts.app')

@section('title', 'Kurz')

@section('content')
    <!-- Content Header (Page header) -->
    <section class="content-header">
        <h1>
            Testy
            <small>Test z Lekce 1</small>
        </h1>
        <ol class="breadcrumb">
            <li><a href="#"><i class="fa fa-dashboard"></i> Domů</a></li>
            <li><a href="#">Kurz 1</a></li>
            <li><a href="#">Lekce 1</a></li>
            <li class="active">Testy</li>
        </ol>
    </section>

    <!-- Main content -->
    <section class="content">

    <div class="nav-tabs-custom" style="cursor: move;">
    <!-- Tabs within a box -->
    <ul class="nav nav-tabs pull-right ui-sortable-handle">
      <li class=""><a href="#revenue-chart" data-toggle="tab" aria-expanded="false">Angličina -> Čeština</a></li>
      <li class="active"><a href="#sales-chart" data-toggle="tab" aria-expanded="true">Čeština -> Angličtina</a></li>
      <li class="pull-left header"><i class="fa fa-inbox"></i> Test č. 1</li>
    </ul>
    <div class="tab-content">
    <div>
                <div class="text-center">
                    <h3>Co v češtině znamená slovo:</h3>
                </div>
                <div class="text-center">
                    <h2 id="word"><strong>"Apple"</strong></h2>
                </div>

            <br>
            <div class="row">
                <div class="col-md-4">
                </div>
                <div class="col-md-4">
                    <input class="form-control input-lg" type="text" placeholder="napište překlad...">
                </div>
                <div class="col-md-4">
                </div>
            </div>

            <br>

            <div class="row">
            <div class="col-md-5">
                </div>
                <div class="col-md-2">
                <button type="button" id="option1" class="btn btn-info btn-block btn-lg">Potvrdit</button>
                </div>
                <div class="col-md-5">
                </div>
            </div>

<br>
<div class="box-footer">
<div class="row">
<div class="col-md-2">
Správně 5 slovíček.
</div>
<div class="col-md-8">
</div>
<div class="col-md-2 text-right">
Špatně 2 slovíčka.
</div>
</div>
  </div>

  
    </section>
    <!-- /.content -->
@endsection