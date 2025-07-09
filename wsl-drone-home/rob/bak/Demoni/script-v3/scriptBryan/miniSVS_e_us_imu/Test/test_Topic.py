import unittest
import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from SubscriberLib.Exceptions import SubtopicNotFoundException
from SubscriberLib.SubtopicValue import SubtopicValue
from SubscriberLib.TopicCollection import TopicCollection
from SubscriberLib.Topic import Topic

class TestTopic(unittest.TestCase):

  def setUp(self) -> None:
    self.topic1 = Topic('Topic1', ['st1.1', 'st1.2'])
    self.topic2 = Topic('Topic2', ['st2.1', 'st2.2'])

    self.all_topic = TopicCollection([self.topic1, self.topic2])

  # Verifics se __getitem__ chimata su una SubtopicCollection ritorna un SubtopicValue
  def test_getitem_subtopic(self):
    print('Test __getitem__ SubtopicCollection') 
    self.assertTrue(isinstance(self.all_topic['Topic1']['st1.1'], SubtopicValue))

  # Verifica che provando ad accedere ad un subtopic non esistente, venga lanciata
  # una SubtopicNotFoundException
  def test_getitem_subtopic_not_found(self):
    print('Test subtopic not found')
    with self.assertRaises(SubtopicNotFoundException) as ctx:
      self.all_topic['Topic1']['st1.5']
    self.assertTrue('"st1.5" is not a valid subtopic' in str(ctx.exception))

  # Verifica del settaggio di un valore di uno specifico subtopic
  def test_set_item(self):
    print('Test set_item')
    self.all_topic['Topic1'].set_item('st1.1', SubtopicValue(timestamp='1111', value='value1.1'))
    self.all_topic['Topic2'].set_item('st2.2', SubtopicValue(timestamp='1111', value='value2.2'))
    
    self.assertEqual(self.all_topic['Topic1']['st1.1'].value, 'value1.1')
    self.assertEqual(self.all_topic['Topic1']['st1.2'].value, None)
    self.assertEqual(self.all_topic['Topic2']['st2.1'].value, None)
    self.assertEqual(self.all_topic['Topic2']['st2.2'].value, 'value2.2')

  # Verifica che durante il settaggio di un subtopic non definito venga langiata 
  # una SubtopicNotFoundException 
  def test_set_item_on_undefined_subtopic(self):
    print('Test set_item su un subtopic non valido')
    with self.assertRaises(SubtopicNotFoundException) as ctx:
      self.all_topic['Topic2'].set_item('st2.5', SubtopicValue(timestamp='1111', value='value2.2'))
    self.assertTrue('"st2.5" is not a valid subtopic' in str(ctx.exception))

  def test_all_valid(self):
    self.topic1.set_item('st1.1', SubtopicValue(timestamp='1111', value='value1.1'))
    self.topic1.set_item('st1.2', SubtopicValue(timestamp='1111', value='value1.2'))
    self.assertTrue(self.topic1.all_valid())
    
    self.topic1['st1.2'].is_valid = False
    self.assertFalse(self.topic1.all_valid())

if __name__ == '__main__':
  unittest.main()