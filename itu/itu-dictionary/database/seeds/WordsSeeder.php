<?php

use Illuminate\Database\Seeder;

use App\Word;

class WordsSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        Word::create([
            'en' => 'home',
            'cs' => 'domov',
        ]);

        Word::create([
            'en' => 'tree',
            'cs' => 'strom',
        ]);

        Word::create([
            'en' => 'payment',
            'cs' => 'platba',
        ]);

        Word::create([
            'en' => 'lesson',
            'cs' => 'lekce',
        ]);

        Word::create([
            'en' => 'notebook',
            'cs' => 'laptop',
        ]);
    }
}
