import unittest
import os, sys
from SubscriberLib.SubtopicValue import SubtopicValue
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from SubscriberLib.Exceptions import TopicNotFoundException
from SubscriberLib.TopicCollection import TopicCollection
from SubscriberLib.Topic import Topic

class TestSubtopicValue(unittest.TestCase):

  def test_creation_without_params(self):
    subtopic_value = SubtopicValue();
    self.assertEqual(subtopic_value.value, None)
    self.assertEqual(subtopic_value.timestamp, None)
    self.assertEqual(subtopic_value.is_valid, True)
  
  def test_creation_with_two_params(self):
    subtopic_value = SubtopicValue(value='1234567890', timestamp='76543');
    self.assertEqual(subtopic_value.value, '1234567890')
    self.assertEqual(subtopic_value.timestamp, '76543')
    self.assertEqual(subtopic_value.is_valid, True)

  # def test_setters(self):
  #   subtopic_value = SubtopicValue();
  #   subtopic_value.is_valid = False


if __name__ == '__main__':
  unittest.main()