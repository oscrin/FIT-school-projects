<?php

use Illuminate\Database\Seeder;

use App\Course;

class CoursesSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        Course::create([
            'user_id' => 1,
            'name' => 'Kurzíček 1',
        ]);
    }
}
