<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;

use Illuminate\Support\Facades\DB;

class MillionaireController extends Controller
{
    public function index($course, $lesson){

        $words = [];

        $db_words = DB::table('words')
            ->join('lesson_word', 'words.id', '=', 'lesson_word.word_id')
            ->where('lesson_word.lesson_id', '=', $lesson)
            ->get();

        $idx = 0;
        $count = count($db_words);

        foreach ($db_words as $dbw){
            $dbw->w1 = $dbw->cs;
            $dbw->w2 = $db_words[(($idx+1 < $count) ? ($idx+1) : ($idx-1))]->cs;
            $dbw->w3 = $db_words[(($idx+2 < $count) ? ($idx+2) : ($idx-2))]->cs;
            $dbw->w4 = $db_words[(($idx+3 < $count) ? ($idx+3) : ($idx-1))]->cs;
            array_push($words, $dbw);
            $idx++;
        }

        return view('millionaire', ['course' => $course, 'lesson' => $lesson, 'words' => $words]);
    }
}
