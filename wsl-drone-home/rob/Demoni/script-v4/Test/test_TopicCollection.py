import unittest
import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from SubscriberLib import SubtopicValue
from SubscriberLib.Exceptions import TopicNotFoundException
from SubscriberLib.TopicCollection import TopicCollection
from SubscriberLib.Topic import Topic

class TestTopicCollection(unittest.TestCase):

  def setUp(self) -> None:
    self.topic1 = Topic('Topic1', ['st1.1', 'st1.2'])
    self.topic2 = Topic('Topic2', ['st2.1', 'st2.2'])

    self.all_topic = TopicCollection([self.topic1, self.topic2])

  # Verifica se __getitem__ ritorna il giusto Topic
  def test_getitem_topic(self):
    print('Test __getitem__ TopicCollection')
    self.assertEqual(self.all_topic['Topic1'], self.topic1)
    self.assertEqual(self.all_topic['Topic2'], self.topic2)
  
  # Verifica che se un topic non è stato definito venga lanciata una TopicNotFoundException
  def test_getitem_topic_not_found(self):
    print('Test __getitem__ con topic non definito')
    with self.assertRaises(TopicNotFoundException) as ctx:
      self.all_topic['Topic3']
    self.assertTrue('Topic "Topic3" not found' in str(ctx.exception))
    
  def test_get_all_subtopics(self):
    print('Test get_all_subtopics')
    self.assertListEqual(['st1.1', 'st1.2'], self.all_topic['Topic1'].get_all_subtopics())

  # Verifica se tutti i timestamps dei valori di un topic siano uguali
  def test_same_timestamps(self):
    self.all_topic['Topic1'].set_item('st1.1', SubtopicValue(timestamp='1111', value='value1.1'))
    self.all_topic['Topic1'].set_item('st1.2', SubtopicValue(timestamp='1111', value='value1.2'))
  
    self.assertTrue(self.all_topic['Topic1'].same_timestamp())

    # Cambio il valore di un timestamp in modo che non siano più uguali
    self.all_topic['Topic1']['st1.1'].timestamp = '2222'
    self.assertFalse(self.all_topic['Topic1'].same_timestamp())

  # Verifica del settaggio di un valore in un subtopic
  def test_is_full(self):
    self.all_topic['Topic1'].set_item('st1.1', SubtopicValue(timestamp='1111', value='value1.1'))
    self.all_topic['Topic1'].set_item('st1.2', SubtopicValue(timestamp='1111', value='value1.2'))

    self.assertTrue(self.all_topic['Topic1'].is_full())  

  def test_reset_all(self):
    print('Test reset_all')
    self.all_topic['Topic1'].set_item('st1.1', SubtopicValue(timestamp='1111', value='value1.1'))
    self.all_topic['Topic1'].set_item('st1.2', SubtopicValue(timestamp='1111', value='value1.2'))

    self.all_topic['Topic2'].set_item('st2.1', SubtopicValue(timestamp='1111', value='value2.1'))
    self.all_topic['Topic2'].set_item('st2.2', SubtopicValue(timestamp='1111', value='value2.2'))

    self.all_topic.reset_all()

    self.assertTrue(self.topic1['st1.1'].is_None)
    self.assertTrue(self.topic1['st1.2'].is_None)
    self.assertTrue(self.topic2['st2.1'].is_None)
    self.assertTrue(self.topic2['st2.2'].is_None)

  def test_all_data_available(self):
    print('Test all_data_available')
    self.all_topic['Topic1'].set_item('st1.1', SubtopicValue(timestamp='1111', value='value1.1'))
    self.all_topic['Topic1'].set_item('st1.2', SubtopicValue(timestamp='1111', value='value1.2'))

    self.all_topic['Topic2'].set_item('st2.1', SubtopicValue(timestamp='1111', value='value2.1'))
    self.all_topic['Topic2'].set_item('st2.2', SubtopicValue(timestamp='1111', value='value2.2'))

    self.assertTrue(self.all_topic.all_data_available())

    # Reset di un subtopic
    self.all_topic['Topic1']['st1.1'].reset()

    self.assertFalse(self.all_topic.all_data_available())

if __name__ == '__main__':
  unittest.main()