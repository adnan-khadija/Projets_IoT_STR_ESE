from django.db import models

# Create your models here.

class Task(models.Model):
    name = models.CharField(max_length=100)
    execution_time = models.IntegerField(help_text="Temps d'exécution en millisecondes")
    deadline = models.IntegerField(help_text="Deadline relative en millisecondes")
    period = models.IntegerField(help_text="Période en millisecondes")
    priority = models.IntegerField(default=0)
    
    def __str__(self):
        return f"{self.name} (C={self.execution_time}, D={self.deadline}, T={self.period})"

class Schedule(models.Model):
    task = models.ForeignKey(Task, on_delete=models.CASCADE)
    start_time = models.IntegerField()
    end_time = models.IntegerField()
    
    def __str__(self):
        return f"{self.task.name}: {self.start_time}-{self.end_time}"
