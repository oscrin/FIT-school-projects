<?php

use Illuminate\Database\Seeder;

class DatabaseSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $this->call(UsersSeeder::class);
        $this->call(CoursesSeeder::class);
        $this->call(LessonsSeeder::class);
        $this->call(WordsSeeder::class);
        $this->call(Words2Seeder::class);
    }
}
