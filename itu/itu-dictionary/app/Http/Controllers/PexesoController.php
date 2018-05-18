<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\DB;

use App\Lesson;

class PexesoController extends Controller
{
    public function index($course, $lesson){

        $words = DB::table('words')
            ->join('lesson_word', function ($join) use ($lesson){
                $join->on('words.id', '=', 'lesson_word.word_id')
                    ->where('lesson_word.lesson_id', '=', $lesson);
            })
            ->get();

        return view('pexeso', ['course' => $course, 'lesson' => $lesson, 'words' => $words]);
    }
}
