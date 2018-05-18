<?php

use Illuminate\Database\Seeder;

use App\Lesson;

class LessonsSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $lesson1 = Lesson::create([
            'course_id' => 1,
            'id_in_course' => 1,
        ]);

        $lesson1->words()->attach([1,2,3,4,5]);

        $lesson2 = Lesson::create([
            'course_id' => 2,
            'id_in_course' => 2,
        ]);

        $lesson2->words()->attach([6,5,7,8,9]);
    }
}
