@extends('layouts.auth-app')

@section('title')
    Obnovit heslo
@endsection

@section('content')

<body class="hold-transition login-page">
<div class="login-box">
    <div class="login-logo">
        <a href="{{ url('/home') }}"><b>Words</b>Training</a>
    </div>
    <!-- /.login-logo -->
    <div class="login-box-body">
        <p class="login-box-msg">Obnovení hesla</p>

        <form action="{{ route('password.email') }}" method="post">
            {{ csrf_field() }}
            <div class="form-group has-feedback{{ $errors->has('email') ? ' has-error' : '' }}">
                <input type="email" class="form-control" placeholder="Email" name="email" value="{{ $email or old('email') }}" required autofocus>
                <span class="glyphicon glyphicon-envelope form-control-feedback"></span>
                @if ($errors->has('email'))
                <span class="help-block">{{ $errors->first('email') }}</span>
                @endif
            </div>
        
        <div class="row">
            <div class="col-xs-5 pull-right">
              <button type="submit" class="btn btn-primary btn-block btn-flat">Obnovit</button>
            </div>
        </div>

        </form>

    </div>
    <!-- /.login-box-body -->
</div>
</body>
@endsection
