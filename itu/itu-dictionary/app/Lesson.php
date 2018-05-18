<?php

namespace App;

use Illuminate\Database\Eloquent\Model;

class Lesson extends Model
{
    public function course(){
        return $this->belongsTo('App\Course');
    }

    public function words(){
        return $this->belongsToMany('App\Word');
    }
}
