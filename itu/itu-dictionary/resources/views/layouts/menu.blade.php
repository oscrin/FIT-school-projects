@php
    function current_page($uri) {
    if(strstr(request()->path(), $uri)){
        return true;
    }
    else{
        return false;
    }
    }

    $lessons = DB::table('lessons')->get();

@endphp
<header class="main-header">
    <nav class="navbar navbar-static-top">
        <div class="container">
            <div class="navbar-header">
                <a href="/" class="navbar-brand"><b>Words</b>Training</a>
                <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar-collapse">
                    <i class="fa fa-bars"></i>
                </button>
            </div>

            <!-- Collect the nav links, forms, and other content for toggling -->
            <div class="collapse navbar-collapse pull-left" id="navbar-collapse">
                <ul class="nav navbar-nav">
                    <li {{ (current_page("home")) ? ' class=active' : '' }}><a href="{{ url('/home') }}"><i class="fa fa-home" aria-hidden="true"></i></a></li>
                    <li {{ (current_page("course")) ? ' class=active dropdown' : 'class="dropdown"' }}>
                        <a href="#" class="dropdown-toggle" data-toggle="dropdown">Kurzy <span class="caret"></span></a>
                        <ul class="dropdown-menu" role="menu">
                            <li><a href="{{ url('/course') }}">Kurz 1</a></li>
                            <li class="divider"></li>
                            <li>
                                <div class="row center-block">
                                    <div class="col-xs-1">
                                    <button type="button" style="width: 130px;" class="btn btn-warning btn-xs" data-toggle="modal" data-target="#vytvoritKurz">Vytvořit kurz</button>
                                    </div>
                                </div>
                            </li>
                            <li class="divider"></li>
                            <li>
                                <div class="row center-block">
                                    <div class="col-xs-1">
                                    <button type="button" style="width: 130px;" class="btn btn-info btn-xs" data-toggle="modal" data-target="#vyhledatKurz">Vyhledat kurz</button>
                                    </div>
                                </div>
                            </li>
                        </ul>
                    </li>
                    <li {{ (current_page("lesson")) ? ' class=active dropdown' : 'class="dropdown"' }}>
                        <a href="#" class="dropdown-toggle" data-toggle="dropdown">Lekce <span class="caret"></span></a>
                        <ul class="dropdown-menu" role="menu">
                            @foreach ($lessons as $lesson)
                                <li><a href='{{ url("/lesson?lekce=$lesson->id") }}'>Lekce {{$lesson->id}}</a></li>
                            @endforeach
                            <li class="divider"></li>
                            <li>
                                <div class="row center-block">
                                    <div class="col-xs-1">
                                    <button type="button" style="width: 130px;" class="btn btn-warning btn-xs" data-toggle="modal" data-target="#vytvoritLekci">Vytvořit lekci</button>
                                    </div>
                                </div>
                            </li>
                        </ul>
                    </li>
                    <li {{ (current_page("millionaire") or current_page("pexeso")) ? ' class=active dropdown' : 'class="dropdown"' }}>
                        <a href="#" class="dropdown-toggle"  data-toggle="dropdown">Výuka <span class="caret"></span></a>
                        <ul class="dropdown-menu" role="menu">
                            @foreach ($lessons as $lesson)
                            <li><a href="{{ url('/1/'.$lesson->id.'/pexeso') }}">L{{ $lesson->id }} Hra Pexeso</a></li>
                            @endforeach
                            <li class="divider"></li>
                            @foreach ($lessons as $lesson)
                            <li><a href="{{ url('/1/'.$lesson->id.'/millionaire') }}">L{{ $lesson->id }} Hra Milionář</a></li>
                            @endforeach
                        </ul>
                    </li>
                    <li {{ (current_page("tests")) ? ' class=active' : '' }}><a href="{{ url('/tests') }}">Testy</a></li>
                </ul>
            </div>
            <!-- /.navbar-collapse -->
            <!-- Navbar Right Menu -->
            <div class="navbar-custom-menu">
                <ul class="nav navbar-nav">
                    {{--
                    <!-- Messages: style can be found in dropdown.less-->
                    <li class="dropdown messages-menu">
                        <!-- Menu toggle button -->
                        <a href="#" class="dropdown-toggle" data-toggle="dropdown">
                            <i class="fa fa-envelope-o"></i>
                            <span class="label label-success">4</span>
                        </a>
                        <ul class="dropdown-menu">
                            <li class="header">You have 4 messages</li>
                            <li>
                                <!-- inner menu: contains the messages -->
                                <ul class="menu">
                                    <li><!-- start message -->
                                        <a href="#">
                                            <div class="pull-left">
                                                <!-- User Image -->
                                                <img src="../../dist/img/user2-160x160.jpg" class="img-circle" alt="User Image">
                                            </div>
                                            <!-- Message title and timestamp -->
                                            <h4>
                                                Support Team
                                                <small><i class="fa fa-clock-o"></i> 5 mins</small>
                                            </h4>
                                            <!-- The message -->
                                            <p>Why not buy a new awesome theme?</p>
                                        </a>
                                    </li>
                                    <!-- end message -->
                                </ul>
                                <!-- /.menu -->
                            </li>
                            <li class="footer"><a href="#">See All Messages</a></li>
                        </ul>
                    </li>
                    <!-- /.messages-menu -->

                    <!-- Notifications Menu -->
                    <li class="dropdown notifications-menu">
                        <!-- Menu toggle button -->
                        <a href="#" class="dropdown-toggle" data-toggle="dropdown">
                            <i class="fa fa-bell-o"></i>
                            <span class="label label-warning">10</span>
                        </a>
                        <ul class="dropdown-menu">
                            <li class="header">You have 10 notifications</li>
                            <li>
                                <!-- Inner Menu: contains the notifications -->
                                <ul class="menu">
                                    <li><!-- start notification -->
                                        <a href="#">
                                            <i class="fa fa-users text-aqua"></i> 5 new members joined today
                                        </a>
                                    </li>
                                    <!-- end notification -->
                                </ul>
                            </li>
                            <li class="footer"><a href="#">View all</a></li>
                        </ul>
                    </li>
                    <!-- Tasks Menu -->
                    <li class="dropdown tasks-menu">
                        <!-- Menu Toggle Button -->
                        <a href="#" class="dropdown-toggle" data-toggle="dropdown">
                            <i class="fa fa-flag-o"></i>
                            <span class="label label-danger">9</span>
                        </a>
                        <ul class="dropdown-menu">
                            <li class="header">You have 9 tasks</li>
                            <li>
                                <!-- Inner menu: contains the tasks -->
                                <ul class="menu">
                                    <li><!-- Task item -->
                                        <a href="#">
                                            <!-- Task title and progress text -->
                                            <h3>
                                                Design some buttons
                                                <small class="pull-right">20%</small>
                                            </h3>
                                            <!-- The progress bar -->
                                            <div class="progress xs">
                                                <!-- Change the css width attribute to simulate progress -->
                                                <div class="progress-bar progress-bar-aqua" style="width: 20%" role="progressbar" aria-valuenow="20" aria-valuemin="0" aria-valuemax="100">
                                                    <span class="sr-only">20% Complete</span>
                                                </div>
                                            </div>
                                        </a>
                                    </li>
                                    <!-- end task item -->
                                </ul>
                            </li>
                            <li class="footer">
                                <a href="#">View all tasks</a>
                            </li>
                        </ul>
                    </li> --}}
                    <!-- User Account Menu -->
                    <li class="dropdown user user-menu">
                        <!-- Menu Toggle Button -->
                        <a href="#" class="dropdown-toggle" data-toggle="dropdown">
                            <!-- The user image in the navbar-->
                            <img src="{{ asset('dist/img/user2-160x160.jpg') }}" class="user-image" alt="User Image">
                            <!-- hidden-xs hides the username on small devices so only the image appears. -->
                            <span class="hidden-xs">{{ Auth::user()->name }}</span>
                        </a>
                        <ul class="dropdown-menu">
                            <!-- The user image in the menu -->
                            <li class="user-header">
                                <img src="{{ asset('dist/img/user2-160x160.jpg') }}" class="img-circle" alt="User Image">

                                <p>
                                    {{ Auth::user()->name }} - {{ Auth::user()->detail }}
                                    <small>Členem od {{ date('d. m. Y', strtotime(Auth::user()->created_at)) }}</small>
                                </p>
                            </li>
                            {{--
                            <!-- Menu Body -->
                            <li class="user-body">
                                <div class="row">
                                    <div class="col-xs-4 text-center">
                                        <a href="#">Followers</a>
                                    </div>
                                    <div class="col-xs-4 text-center">
                                        <a href="#">Sales</a>
                                    </div>
                                    <div class="col-xs-4 text-center">
                                        <a href="#">Friends</a>
                                    </div>
                                </div>
                                <!-- /.row -->
                            </li>
                            --}}
                            <!-- Menu Footer-->
                            <li class="user-footer">
                                <div class="pull-left">
                                    <a href="#" class="btn btn-default btn-flat">Profil</a>
                                </div>
                                <div class="pull-right">
                                    <a href="{{ route('logout') }}" class="btn btn-default btn-flat">Odhlásit se</a>
                                </div>
                            </li>
                        </ul>
                    </li>
                </ul>
            </div>
            <!-- /.navbar-custom-menu -->
        </div>
        <!-- /.container-fluid -->
    </nav>
</header>
<!-- Modal -->
<div class="modal fade" id="vytvoritKurz" role="dialog">
    <div class="modal-dialog">
    
      <!-- Modal content-->
      <div class="modal-content">
        <div class="modal-header">
          <button type="button" class="close" data-dismiss="modal">&times;</button>
          <h4 class="modal-title">Vytvořit Kurz</h4>
        </div>
        <div class="modal-body">
          <h4>Název kurzu:</h4>
          <input class="form-control input-lg" type="text" placeholder="zde napište název...">
        </div>
        <div class="modal-footer">
          <button type="button" class="btn btn-default" data-dismiss="modal">Zrušit</button>
          <button type="button" class="btn btn-success" data-dismiss="modal">Přidat kurz</button>
        </div>
      </div>
      
    </div>
  </div>   
  <div class="modal fade" id="vyhledatKurz" role="dialog">
    <div class="modal-dialog">
    
      <!-- Modal content-->
      <div class="modal-content">
        <div class="modal-header">
          <button type="button" class="close" data-dismiss="modal">&times;</button>
          <h4 class="modal-title">Vyhledat Kurz</h4>
        </div>
        <div class="modal-body">
        <h4>Název hledaného kurzu:</h4>
          <input class="form-control input-md" type="text" placeholder="zde napište název...">
          <br>
          <div class="form-group">
                  <label>Vyberte z nalezených</label>
                  <select class="form-control">

                  </select>
                </div>
        </div>
        <div class="modal-footer">
        <button type="button" class="btn btn-default" data-dismiss="modal">Zrušit</button>
          <button type="button" class="btn btn-success" data-dismiss="modal">Přidat nalezený kurz</button>
        </div>
      </div>
      
    </div>
  </div>    
  <div class="modal fade" id="vytvoritLekci" role="dialog">
    <div class="modal-dialog">
    
      <!-- Modal content-->
      <div class="modal-content">
        <div class="modal-header">
          <button type="button" class="close" data-dismiss="modal">&times;</button>
          <h4 class="modal-title">Vytvořit Lekci</h4>
        </div>
        <div class="modal-body">
            <h4>Název lekce:</h4>
          <input class="form-control input-lg" type="text" placeholder="zde napište název...">
        </div>
        <div class="modal-footer">
        <button type="button" class="btn btn-default" data-dismiss="modal">Zrušit</button>
          <button type="button" class="btn btn-success" data-dismiss="modal">Přidat lekci</button>
        </div>
      </div>
      
    </div>
  </div>    