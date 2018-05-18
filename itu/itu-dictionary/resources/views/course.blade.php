@extends('layouts.app')

@section('title', 'Kurz')

@section('content')
    <!-- Content Header (Page header) -->
    <section class="content-header">
        <h1>
            Přehled kurzu
            <small>Kurz 1</small>
        </h1>
        <ol class="breadcrumb">
            <li><a href="#"><i class="fa fa-dashboard"></i> Domů</a></li>
            <li><a href="#">Kurzy</a></li>
            <li><h3><a href="#">Kurz 1</a></h3></li>
            <li class="active">Přehled</li>
        </ol>
    </section>

    <!-- Main content -->
    <section class="content">
    <div class="box">
        <div class="box-header">
            <div class="col-md-2"><h4><strong>Kurz 1</strong></h4></div>
            <div class="col-md-4"></div>
            <div class="col-md-2">
                <div style="margin-top: 5px;">
                    <button type="button" class="btn btn-block btn-danger btn-sm">Odstranit kurz</button>
                </div>
            </div>
            <div class="col-md-2">
                <div style="margin-top: 5px;">
                    <button type="button" class="btn btn-block btn-warning btn-sm">Přidat lekci do kurzu</button>
                </div>
            </div>
            <div class="col-md-2">
                <div style="margin-top: 5px;">
                    <button type="button" class="btn btn-block btn-success btn-sm">Změnit název kurzu</button>
                </div>
            </div>
        </div>
        <hr>
        <div class="box-body">
            <div style="padding-left: 15px; padding-right: 15px; padding-bottom: 15px;">
                <h4><strong>Popis</strong></h4>
                Tento kurz je dobrovolnou součástí předmětu AIT na Vysokém učení technickém. <br> <br>
                Obsahuje prozatím 2 lekce přístupné studentům.
            </div>
        </div>
    </div>

    <div class="box">
            <div class="box-header">
                <div class="col-md-2 "><h4><strong>Lekce 1</strong></h4></div>
                <div class="col-md-6"></div>
                <div class="col-md-2">
                    <div style="margin-top: 5px;">
                        <button type="button" class="btn btn-block btn-info btn-sm"><a  style="text-decoration: none;" href="{{ url('/lesson') }}">Upravit slovíčka v lekci</a></button>
                    </div>
                </div>
                <div class="col-md-2">
                    <div style="margin-top: 5px;">
                        <button type="button" class="btn btn-block btn-warning btn-sm">Změnit název lekce</button>
                    </div>
                </div>
            </div>
            <!-- /.box-header -->
            <div class="box-body no-padding">

            </div>
            <!-- /.box-body -->
            <div class="box-footer">
                Celkem 15 slovíček.
              </div>
        </div>

        <div class="box">
        <div class="box-header">
        <div class="col-md-2 "><h4><strong>Lekce 2</strong></h4></div>
        <div class="col-md-6"></div>
        <div class="col-md-2">
            <div style="margin-top: 5px;">
                <button type="button" class="btn btn-block btn-info btn-sm"><a style="text-decoration: none;" href="{{ url('/lesson') }}">Upravit slovíčka v lekci</a></button>
            </div>
        </div>
        <div class="col-md-2">
            <div style="margin-top: 5px;">
                <button type="button" class="btn btn-block btn-warning btn-sm">Změnit název lekce</button>
            </div>
        </div>
    </div>
            <!-- /.box-header -->
            <div class="box-body no-padding">

            </div>
            <!-- /.box-body -->
            <div class="box-footer">
                Celkem 10 slovíček.
              </div>
        </div>

    </section>
    <!-- /.content -->
@endsection