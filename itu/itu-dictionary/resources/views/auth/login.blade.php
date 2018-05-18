@extends('layouts.auth-app')

@section('title')
    Přihlásit se
@endsection

@section('content')
<body class="hold-transition login-page">
<div class="login-box">
    <div class="login-logo">
        <a href="{{ url('/home') }}"><b>Words</b>Training</a>
    </div>
    <div class="login-box-body">
        <p class="login-box-msg">Přihlásit se</p>

        <form action="{{ route('login') }}" method="post">
            {{ csrf_field() }}
            <div class="form-group has-feedback{{ $errors->has('email') ? ' has-error' : '' }}">
                <input type="email" class="form-control" placeholder="Email" name="email" value="{{ old('email') }}" required autofocus>
                <span class="glyphicon glyphicon-envelope form-control-feedback"></span>
                @if ($errors->has('email'))
                <span class="help-block">{{ $errors->first('email') }}</span>
                @endif
            </div>

            <div class="form-group has-feedback{{ $errors->has('password') ? ' has-error' : '' }}">
                <input name="password" type="password" class="form-control" placeholder="Heslo" required>
                <span class="glyphicon glyphicon-lock form-control-feedback"></span>
                @if ($errors->has('password'))
                    <span class="help-block">{{ $errors->first('password') }}</span>
                @endif
            </div>
            
            <div class="row">
                <div class="col-xs-8">
                    <div class="checkbox icheck">
                        <label>
                            <input type="checkbox" name="remember"{{ old('remember') ? ' checked' : '' }}> Zapamatovat přihlášení
                        </label>
                    </div>
                </div>
                <div class="col-xs-4">
                    <button name="login_btn" type="submit" class="btn btn-primary btn-block btn-flat">Přihlásit se</button>
                </div>
            </div>
        </form>

        <a href="{{ route('password.request') }}">Zapomněli jste heslo?</a><br>
        <a href="{{ route('register') }}" class="text-center">Registrace</a>

    </div>
</div>
@endsection